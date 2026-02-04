import json
import sys, getopt, os
from dotenv import load_dotenv

from guidance import system, user, assistant, gen
from guidance import json as gen_json
from guidance.models import Transformers, OpenAI

def main():
    tasks = sys.argv[1]
    decorators = sys.argv[2]
    prompt = sys.argv[3]
    path = sys.argv[4]

    ROUTE = 'C:/Users/pablo/Desktop/Uni/Master/TFM/BTGenerator/Content/TFM/JSONs/BTschema.json'

    schema = ''
    with open(ROUTE, 'r') as f:
        schema = json.load(f)
        schema = json.dumps(schema, indent = 4)

    # load_dotenv()
    # print(os.getenv("OPENAI_API_KEY"))
    #phi_lm = OpenAI('gpt-4.1', api_key = os.getenv("OPENAI_API_KEY")) # Límite de tokens

    phi_lm = Transformers("Qwen/Qwen2.5-Coder-7B-Instruct") # Antes: "microsoft/Phi-4-mini-instruct", pero es demasiado pequeño para el schema tan complejo (JSONs del 1 al 4).
    lm = phi_lm

    with system():
        lm += f'''
You are an expert in Unreal Engine 5 and designing behaviors in videogames using Behavior Trees ( BT ).

You have to create a BT and the Blackboard ( BB ) from the user's description following the format given by the JSON schema.

In order to create de BB, use carefuly the info given by the user prompt (for example, if the user need a NPC that has to go to a random point, is very needed an entry in the blackboard for store that point, like "Point" : "Vector3").

The BT is composed of "Node" objects. ("Node" : {{...}})
The type of the nodes can ONLY be: Sequence, Selector, Parallel or Task.
They can have one Decorator in the "Decorators" list. Decorators are conditionals, and only has to be used when the execution of a node depends on that condition. The Decorators are objects with the form "Decorators' ID" : "BB Entry".
If the node's type is "Sequence", "Selector" or "Parallel" it MUST have AT LEAST ONE child node (it CAN'T be a leaf node), and they HAVE to be defined in the "Nodes" list as a "Node" object ("Node" : {{...}}).
If the node's type is "Task" it CAN'T have child nodes (it SHOULD be a leaf node). If the task need to use an entry from the BB, it SHOULD be referencered in the "BlackboardEntries" list. The tasks MUST have a Task ID from the list of Tasks' IDs and it HAS TO BE in the "Task" field of the node.

The "Sequence" nodes executes their child nodes in order.
The "Selector" nodes executes the first child that meets the conditions.
The "Task" node is a node that executes an action. 

The lists of Tasks and Decorators are formed by the different IDs separated by ",". If a list only contains a "_" is because is empty, so you CAN'T use it. 

You can ONLY use the following list of Tasks' IDs: {tasks}

You can ONLY use the following list of Decorators' IDs: {decorators}

Don't stop until the behaviour does EXACTLY EVERYTHING of the description.
        '''

    with user():
        lm += prompt

    with assistant():
        lm += gen_json(name = 'BT', schema = schema)

    loaded_json = json.loads(lm["BT"])
    print(json.dumps(loaded_json, indent=4))
    f = open(f'JSONs/{path}.json', 'w')
    f.write(json.dumps(loaded_json, indent=4))
    f.close()
    s = "NPC that, constantly, select a random point from the map, goes to the point and waits a time."

    
if __name__ == "__main__":
    main()
