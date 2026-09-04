from dotenv import load_dotenv
from pathlib import Path

from langchain_mistralai import ChatMistralAI
from langchain_ollama import ChatOllama

from langchain_core.documents import Document
from langchain_community.vectorstores import FAISS
from langchain_huggingface import HuggingFaceEmbeddings

from dataclasses import dataclass, field
from typing import List, Optional
import re
import json
import sys, getopt, os
import time



class Node:
    pass


@dataclass
class Composite(Node):
    type : str = ""
    children: List[Node] = field(default_factory=list)
    decorator: Optional[str] = None
    decoratorEntry: list[str] = field(default_factory=list)

    id : int = -1


@dataclass
class Action(Node):
    name: str = ""
    decorator: Optional[str] = None
    decoratorEntry: list[str] = field(default_factory=list)
    blackboardEntries: list[str] = field(default_factory=list)

    id: int = -1

class ParserError(Exception):
    pass


class BehaviorTreeParser:
    def parse(self, pseudocode: str, actions : str, bbactions : str, conditionals : str, BBEntries : List[dict], prompt : str) -> str:
        all_actions = actions + bbactions
        
        lines = self._preprocess(pseudocode)

        stack = []
        root = None

        pending_condition = None
        in_else = False

        for i, line in enumerate(lines):
            tokens = line.split()

            if not tokens:
                continue

            keyword = tokens[0]

            if keyword == "SEQUENCE" or keyword == "SELECTOR":
                node = Composite()
                node.type = keyword

                if pending_condition and not in_else:
                    node.decorator = pending_condition
                    pending_condition = None

                elif pending_condition and in_else:
                    pending_condition = None
                    in_else = False

                self._add_to_parent(stack, node, i)
                stack.append((node, "children"))
                continue

            if keyword == "IF":
                if len(tokens) < 2:
                    raise ParserError(f"Line {i}: IF missing condition name")

                if tokens[1] not in conditionals:
                    raise ParserError(f"Unknown conditional in line {i}. Conditional {tokens[1]} is not in the conditionals list.")

                pending_condition = tokens[1]
                in_else = False
                continue

            if keyword == "THEN":
                continue

            if keyword == "ELSE":
                in_else = True
                continue

            if keyword == "DO":
                if len(tokens) < 2:
                    raise ParserError(f"Line {i}: DO missing action")
                
                if tokens[1] not in all_actions:
                    raise ParserError(f"Unknown action in line {i}. Action {tokens[1]} is not in the actions list.")

                node = Action(name=tokens[1])

                if pending_condition and not in_else:
                    node.decorator = pending_condition
                    pending_condition = None

                elif pending_condition and in_else:
                    pending_condition = None
                    in_else = False

                self._add_to_parent(stack, node, i)
                continue

            if keyword == "END":
                if not stack:
                    raise ParserError(f"Line {i}: END without block")

                node, _ = stack.pop()

                if not stack:
                    if root is not None:
                        raise ParserError("Multiple roots detected")
                    root = node

                continue

            raise ParserError(f"Line {i}: Unknown token {keyword}")

        if stack:
            raise ParserError("Unclosed blocks detected")

        if root is None:
            raise ParserError("No root node generated")

        agent = BlackboardAssignmentAgent()

        agent.assignBlackboard(root, bbactions, conditionals, BBEntries, prompt)

        return self._toJSON(root, bbactions, conditionals, BBEntries, pseudocode, prompt)
    
    def _toJSON(self, root : Node, bbactions : str, conditionals : str, BBEntries : list[dict], pseudocode : str, prompt : str) -> dict:
        decorator = []
        if root.decorator is not None:
            if len(root.decoratorEntry) > 0:
                decorator.append({root.decorator: root.decoratorEntry[0]})

        if isinstance(root, Action):
            entry = root.blackboardEntries

            return {
                "Node": {
                    "Type": "Task",
                    "Task": root.name,
                    "Decorators": decorator,
                    "BlackboardEntries": entry
                }
            }

        children = [self._toJSON(child, bbactions, conditionals, BBEntries, pseudocode, prompt) for child in root.children]

        return {
            "Node": {
                "Type": root.type.capitalize(),
                "Decorators": decorator,
                "Nodes": children
            }
        }

    def _preprocess(self, pseudocode: str) -> list[str]:
        lines = pseudocode.split("\n")
        return [line.strip() for line in lines if line.strip()]

    def _add_to_parent(self, stack, node, line_number):
        if not stack:
            return

        parent, branch = stack[-1]
        getattr(parent, branch).append(node)

    def _get_action_and_description(self, actions : str, action_name : str) -> tuple[str, str]:
        pairs = [p.strip() for p in actions.split(",")]

        for pair in pairs:
            if " : " in pair:
                action, desc = pair.split(" : ", 1)
                if action.strip() == action_name:
                    return action.strip() + " : " + desc.strip()

        return None

class BlackboardAssignmentAgent:

    def __init__(self):

        self.llm = ChatOllama(
            model="llama3:8b",
            temperature=0.0,
            timeout=20
        )

    def buildSystemPrompt(self):

        return """
You are an expert in Unreal Engine Behavior Trees.

Your task is NOT to understand the whole game.

Your ONLY task is assigning Blackboard variables to Behavior Tree nodes.

You will receive

- the complete behavior tree
- the description of every action that needs a Blackboard variable
- the Blackboard variables

Your goal is assigning Blackboard variables to every action that needs them.

Rules

1. Never invent variables.
2. Variables MUST belong to the Blackboard list.
3. An action usually consumes the variable produced by previous actions.
4. If two variables have the same type, choose the one that best matches the previous actions.
5. Use the whole tree to infer data flow.
6. Do not assign variables to actions that do not require them.
7. Decorators should receive exactly one Blackboard key if they need one.
8. Assign variables to EVERY action and decorator that needs them.

Output ONLY valid JSON.

Example Input:
Actions : grab_key : Stores the key in the inventory, move_to : Moves to a position, open_door: Open a door
Variables: ["Key" : "Object", "Door" : "Object", "Enemy_position" : "Vector3"]
Tree:
SEQUENCE
    DO move_to#1
    DO grab_key#1
    DO move_to#2
    DO open_door#1 [Decorator=IsNear?#1]
    DO wave#1

Output format: 
{
    "Actions": {
        "move_to#1":["Key"],
        "grab_key#1":["Key"],
        "move_to#2":["Door"],
        "open_door#1":["Door"]
    },

    "Decorators": {
        "IsNear?#1":["Door"]
    }
}

No explanations.
"""

    def serializeTree(self, node, depth=0, actionCounters = None, decoratorCounters = None, lines=None):

            if actionCounters is None or decoratorCounters is None:
                actionCounters = {}
                decoratorCounters = {}

            if lines is None:
                lines = []

            tabs = "    " * depth

            if isinstance(node, Composite):
                decorator = ""
                if node.decorator is not None:
                    count = decoratorCounters.get(node.decorator, 0) + 1
                    decoratorCounters[node.decorator] = count

                    node.id = count

                    decorator = f" [Decorator={node.decorator}#{count}]"

                lines.append(f"{tabs}{node.type}{decorator}")

                for child in node.children:
                    self.serializeTree(child, depth + 1, actionCounters, decoratorCounters, lines)

                return "\n".join(lines)

            if isinstance(node, Action):
                count = actionCounters.get(node.name, 0) + 1
                actionCounters[node.name] = count

                node.id = count

                decorator = ""

                if node.decorator is not None:

                    decCount = decoratorCounters.get(node.decorator, 0) + 1
                    decoratorCounters[node.decorator] = decCount

                    decorator = f" [Decorator={node.decorator}#{decCount}]"

                lines.append(f"{tabs}DO {node.name}#{count}{decorator}")

                return "\n".join(lines)

    def buildActionDescriptions(self, bbactions, conditionals):

            txt = []

            for pair in bbactions.split(","):

                pair = pair.strip()

                if " : " in pair:

                    txt.append(pair)

            txt.append("")

            txt.append("Decorators")

            for pair in conditionals.split(","):

                pair = pair.strip()

                if " : " in pair:

                    txt.append(pair)

            return "\n".join(txt)

    def buildUserPrompt(self, root, bbactions, decorators, blackboard, actionPrompt):

        tree = self.serializeTree(root)

        actions = self.buildActionDescriptions(
            bbactions,
            decorators
        )

        return f"""
GLOBAL TASK

{actionPrompt}

----------------------

BEHAVIOR TREE

{tree}

----------------------

AVAILABLE ACTIONS

{actions}

----------------------

BLACKBOARD

{json.dumps(blackboard, indent=4)}

Return ONLY JSON.
"""

    def assignBlackboard(self, root, bbactions, decorators, blackboard, globalPrompt):

        system = self.buildSystemPrompt()

        user = self.buildUserPrompt(root, bbactions, decorators, blackboard, globalPrompt)

        messages = [
            ("system", system),
            ("human", user)
        ]

        response = self.llm.invoke(messages).content

        messages.append(('assistant', response))
        messages.append(("human", "Verify that ALL the actions that needs a Blackboard variable have a variable. Ensure that ALL the variables exists and not invent new ones. Respond me ONLY with the output format and do not give me explanations or plain text."))

        response = self.llm.invoke(messages).content

        result = self._parseJSON(response)

        retries = 0

        while (not self._validateResult(result, blackboard) and retries < 3):

            retries += 1

            messages.append(("assistant", response))

            messages.append((
                "human",
                "The previous answer is invalid. "
                "Return ONLY valid JSON. "
                "Do not invent Blackboard variables."
            ))

            response = self.llm.invoke(messages).content

            result = self._parseJSON(response)

        if result is None:
            raise RuntimeError("Impossible to assign Blackboard variables.")

        print("BB response: ", response)

        self._applyRecursive(root, result)

    def _parseJSON(self, text):
        match = re.search(r"\{.*\}", text,re.DOTALL)

        if match is None:
            return None

        try:
            return json.loads(match.group())
        except:
            return None

    def _validateResult(self, result, blackboard):

        if result is None:
            return False

        if "Actions" not in result:
            return False

        if "Decorators" not in result:
            return False

        validNames = set()

        for bb in blackboard:
            for key in bb.keys():
                validNames.add(key)

        for variables in result["Actions"].values():
            if not isinstance(variables, list):
                return False

            for v in variables:
                if v not in validNames:
                    return False

        for variables in result["Decorators"].values():
            if not isinstance(variables, list):
                return False
            
            for v in variables:
                if v not in validNames:
                    return False

        return True

    def _applyRecursive(self, node, result):
        if isinstance(node, Composite):
            key = ""   
            if node.decorator is not None:
                key = f"{node.decorator}#{node.id}"

            node.decoratorEntry = result["Decorators"].get(key, [])

            for child in node.children:
                self._applyRecursive(child, result)

            return

        key = f"{node.name}#{node.id}"

        node.blackboardEntries = result["Actions"].get(key,[])
    
def load_vector_store() -> FAISS:

    examples_path = os.getenv('EXAMPLES_ROOT')

    files = []
    if os.path.exists(examples_path):
        files = [os.path.join(dirpath,f) for (dirpath, _, filenames) in os.walk(examples_path) for f in filenames]
    
    docs = []
    for f in files:
        e = open(f, 'r')
        j = json.load(e)
        e.close()

        j.pop("BT")

        j = json.dumps(j, indent = 4)

        docs.append(Document(page_content = j, metadata = {'source' : f}))

    model_kwargs = {"device": "cuda"}
    embeddings = HuggingFaceEmbeddings(model_name="all-MiniLM-L6-v2", model_kwargs = model_kwargs)
    vector_store = FAISS.from_documents(docs, embeddings)
    
    return vector_store

def retrieve_examples(vector_store : FAISS, query : str, k : int = 3) -> str:
    if vector_store is None:
            return ""

    docs = vector_store.similarity_search(query, k=k)
    context = "\n\n".join([doc.page_content for doc in docs])
    return context

def main():
    startTime = time.time()

    tasks = sys.argv[1]
    bbtasks = sys.argv[2]
    decorators = sys.argv[3]
    prompt = sys.argv[4]
    path = sys.argv[5]
    entries = sys.argv[6]

    BBEntries = json.loads(entries)

    load_dotenv()

    vector_store = load_vector_store()

    examples = retrieve_examples(vector_store, prompt, 5)

    system_prompt = f"""
You are an expert in behavior trees and algorithm design.

Your task is to convert a high-level task description into a structured pseudocode algorithm that can later be transformed into a Behavior Tree.

Input:
    * A task description

Requirements:
    * Use ONLY the provided actions and ONLY the provided conditions. Do NOT invent new ones. You DON'T HAVE to use ALL the provided actions and conditions, USE ONLY the necessary ones.
    * Produce a deterministic and unambiguous pseudocode.
    * The structure must be easy to parse programmatically.
    * Explicitly represent:
        - Sequence (ordered execution)
        - Selector (fallback / OR logic / conditionals)
        - Conditions (checks)
        - Actions (leaf nodes)
    * Avoid natural language ambiguity. Use a strict format.

Pseudocode Format Rules:
    * Use uppercase keywords: SEQUENCE, SELECTOR, IF, THEN, ELSE, END
    * You don't have to use all the keywords, only use the necessary ones.
    * Indentation must reflect hierarchy
    * Each condition must be written as: IF condition_name. Use ONLY the names from the "conditions" list, DO NOT use other name and DO NOT invent others. 
    * Each action must be written as: DO action_name. Use ONLY the names from the "actions" list.
    * ONLY blocks of SEQUENCE and SELECTOR must always be explicitly closed with END. DO NOT use the END keyword in other blocks, not even in the IF ELSE statements.
    * No free text explanations, only pseudocode

Behavior Tree Mapping:
    * SEQUENCE: executes children in order until one fails. Perfect for when actions need to be performed in a specific order.
    * SELECTOR: executes children until one succeeds. Perfect for when you have to choose between actions.
    * IF condition THEN ... ELSE ... : conditional branching

Reasoning Procedure:
    Before generating the pseudocode, internally perform the following reasoning.

    Do NOT output these steps.

    Phase 1
    Understand the global objective.

    Identify:

    * the final goal;
    * mandatory subtasks;
    * optional subtasks;
    * possible dependencies between tasks.

    Phase 2
    Analyse the available actions.

    For every action determine internally:

    * what it achieves;
    * when it is useful;
    * whether another action already achieves the same purpose.

    Discard unnecessary actions.

    Phase 3
    Analyse the available conditions.

    Determine which conditions are actually necessary for solving the task.

    Ignore irrelevant conditions.

    Phase 4
    Construct the execution plan.

    Prefer:

    * the minimum number of actions;
    * the minimum tree depth;
    * deterministic execution;
    * reusable sequences.

    Only introduce a Selector when alternative behaviours are required.

    Only introduce an IF when behaviour explicitly depends on a condition.

    Phase 5
    Verify the solution.

    Internally verify that:

    * every action belongs to the available actions;
    * every condition belongs to the available conditions;
    * every IF has a THEN;
    * every SEQUENCE and SELECTOR has a matching END;
    * no END closes an IF or an ELSE;
    * the algorithm solves the requested task;
    * there are no redundant actions;
    * there are no unreachable branches.

    Only after all checks succeed, generate the final pseudocode.

    Do NOT output your reasoning.

Output:
Return ONLY the pseudocode, no explanations.

Example Input:
Task: "Open a locked door"
Actions: [find_key, move_to_door, open_door]
Conditions: [has_key, door_is_locked]

Example Output:
```
SEQUENCE
    IF has_key THEN
        DO move_to_door
        DO open_door
    ELSE
        SEQUENCE
            DO find_key
            DO move_to_door
            DO open_door
        END
END
```

The following examples are not documentation.

They are demonstrations of valid planning patterns.

Before generating the solution:

1. Find the example that is most similar to the current task.
2. Reuse its planning structure whenever possible.
3. Adapt only the actions and conditions that differ.
4. Preserve the overall control flow unless the task requires a different one.

Do not copy examples blindly.
Adapt them to the current task.

The "prompt" field is the user prompt, and the "code" field is the pseudocode generated by that user prompt:

{examples}

List of actions ("actions" list): {tasks + bbtasks}

List of conditions ("conditions" list) : {decorators}
"""

    llm = ChatMistralAI(model="mistral-large-latest", temperature=0.0, timeout=15)

    messages = [
        ("system", system_prompt),
        ("human", prompt)
    ]

    response = llm.invoke(messages)

    messages.append(("assistant", response.content))
    messages.append(("human", "Verify if your pseudocode is made only with the actions and conditions available. Responde me with ONLY the final pseudocode and no explanations or plain text"))
    
    response = llm.invoke(messages)

    code = re.search(r"```(.*?)```", response.content, re.DOTALL)

    if code != None:
        code = code.group(1)
    else:
        code = response.content


    print(code)

    parser = BehaviorTreeParser()
    maxRetries = 3
    retries = 0
    parsed = False
    bt = ""


    while not parsed and retries < maxRetries:
        try: 
            bt = parser.parse(code, tasks, bbtasks, decorators, BBEntries, prompt)
            parsed = True
        except ParserError as e:
            retries += 1
            
            messages.append(("assistant", code))
            messages.append(("human", f"The last pseudocode gave me the following error while parsing: {e}. Fix it and give me only the pseudocode."))

            try:
                response = llm.invoke(messages)
            except:
                llm_fallback = ChatOllama(model = "llama3:8b", temperature= 0.0, timeout = 10)
                response = llm_fallback.invoke(messages)
                print(response.content)

            
            code = re.search(r"```(.*?)```", response.content, re.DOTALL)
            if code != None:
                code = code.group(1)
            else:
                code = response.content
            
    print(bt)
    print(json.dumps(bt, indent = 4))

    j = {
        "code" : code,
        "BT" : {
            "Blackboard" : BBEntries,
            "Root" : bt
        }
    }

    endTime = round(time.time() - startTime, 2)

    if(os.path.exists(f'{os.getenv('JSON_ROOT')}{path}.json')):
        e = open(f'{os.getenv('JSON_ROOT')}{path}.json', 'r')
        prevBT = json.load(e)
        e.close()
        j["Tries"] = prevBT["Tries"] + 1
        j["Time"] = prevBT["Time"] + endTime
    else:
        j["Tries"] = 1
        j["Time"] = endTime

    
    f = open(f'{os.getenv('JSON_ROOT')}{path}.json', 'w')
    f.write(json.dumps(j, indent=4))
    f.close()

    
if __name__ == "__main__":
    main()
