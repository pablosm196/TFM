from datasets import load_dataset
import os
from tqdm import tqdm
import xml.etree.ElementTree as ET
import re
import json

NODE_TYPE_MAP = {
    "Fallback": "Selector",
    "Sequence": "Sequence",
    "Action": "Task",
    "SubTree": "Task"
}

def parse_node(xml_node: str, conditions: str) -> str:
    tag = xml_node.tag

    if tag == "Condition":
        return None 
    json_type = NODE_TYPE_MAP.get(tag)
    if json_type is None:
        raise ValueError(f"Tipo de nodo XML no reconocido: {tag}")

    node_obj = {
        "Type": json_type,
        "Decorators": [],
        "Nodes": []
    }
    if json_type == "Task":
        node_obj["Task"] = xml_node.attrib.get("ID", tag)

    if "ID" in xml_node.attrib:
        node_obj["Name"] = xml_node.attrib["ID"]

    cond = None
    for child in xml_node:
        if child.tag == "Condition":
            cond_id = child.attrib.get("ID", "UnnamedCondition")
            conditions.add(cond_id)
            cond = {f"{cond_id}?": cond_id}
            #node_obj["Decorators"].append({f"{cond_id}?": cond_id})
        else:
            child_obj = parse_node_wrapper(child, conditions)
            if cond is not None:
                child_obj["Decorators"].append(cond)
                cond = None
            node_obj["Nodes"].append(child_obj)

    return node_obj

def parse_node_wrapper(xml_node: str, conditions: set) -> str:
    key = xml_node.tag
    parsed = parse_node(xml_node, conditions)

    if parsed is None:
        raise RuntimeError("Condition no debería envolverse como nodo")

    return parsed

def preprocess_xml(s: str) -> str:
    s = clean_xml(s)
    s = re.sub(r'\bDescendingPriority\b', "Sequence", s)
    s = re.sub(r'\bConditional\b', "Condition", s)
    s = re.sub(r'\bSubtree\b', "SubTree", s)
    s = re.sub(r"<\s*Variation\b[^>]*/>", "", s)
    s = re.sub('<<', '<', s)
    s = re.sub('>>', '>', s)
    s = re.sub(r"<\s*FallBack\b", "<Fallback", s)
    s = re.sub(r"</\s*FallBack\s*>", "</Fallback>", s)


    pattern = r"(?m)^\s*([A-Za-z_][A-Za-z0-9_]*)\s*>"
    replacement = r"<\1>"
    s = re.sub(pattern, replacement, s)

    pattern = r'(\b\w+\b)\s*=\s*([A-Za-z0-9_][A-Za-z0-9_ ]*[A-Za-z0-9_])'
    replacement = r'\1="\2"'
    s = re.sub(pattern, replacement, s)

    s = re.sub(r"<\s*SubTree\b[^>/]*>", "", s)
    s = re.sub(r"</\s*SubTree\s*>", "", s)

    s = re.sub(r'\s+=\s+', '="', s)
    s = re.sub(r'\s{2,}', ' ', s)


    empty_tags = ["Action", "Condition", "SubTree"]

    for tag in empty_tags:
        pattern = fr"<{tag}\b([^>/]*)>"
        replacement = fr"<{tag}\1 />"
        s = re.sub(pattern, replacement, s)

    lines = s.splitlines()
    cleaned = []
    for line in lines:
        line = line.replace("\xa0", " ")
        line = line.replace("\t", "    ")
        cleaned.append(line)
    s =  "\n".join(cleaned)

    try:
        ET.fromstring(s)
    except Exception:
        print(s) 

    return s

def clean_xml(s: str) -> str:
    s = s.replace("\ufeff", "")
    s = s.replace("\x00", "")
    s = s.replace("\u200b", "")
    s = s.replace("\u200c", "")
    s = s.replace("\u200d", "")
    s = s.replace("\u202c", "")
    s = s.replace("\u202d", "")
    return re.sub(
        r"[^\x09\x0A\x0D\x20-\uD7FF\uE000-\uFFFD]",
        "",
        s
    )

def parsePseudoCode(bt: dict, level : int = 0) -> tuple[str, bool]:
    code = ""

    t = bt["Type"]
    hasDecorator = len(bt["Decorators"]) != 0

    if hasDecorator:
        level += 1
        for key, _ in bt["Decorators"][0].items():
            code += "\t" * (level + 1) + f"IF {key} THEN\n"

    if t != "Task":
        code += "\t" * (level + 1) + f"{t.upper()}\n"
    else:
        code += "\t" * (level + 1) + f"DO {bt["Task"]}\n"

    cond = False
    for ch in bt["Nodes"]:
        if cond: 
            code += "\t" * (level + 1) + "ELSE\n"
        c, cond = parsePseudoCode(ch, level + 1)
        code += c


    if t != "Task":
        code += "\t" * (level + 1) + "END\n"


    return code, hasDecorator

    

def parseBT(xml_string: str) -> str:
    conditions = set()

    xml_string = preprocess_xml(xml_string)

    root = ET.fromstring(xml_string)

    first_child = list(root)[0]

    json_tree = {
        "Blackboard": [],
        "Root" : {
            "Node": parse_node_wrapper(first_child, conditions)
        }
    }

    for cond in conditions:
        json_tree["Blackboard"].append({cond : "Boolean"})

    return json_tree

def main():
    if not os.path.exists('JSONs'):
        os.mkdir('./JSONs')
    path = './JSONs/Example_'

    ds = load_dataset("ArtemLykov/LLM_BRAIn_dataset")

    i = 0
    for bt in tqdm(ds['train']):
        try:

            p = parseBT(bt['output'])
            p = str(p)
            p = p.replace('\'', '\"')
            j = json.loads(str(p))

            c = "```\n"
            code, _ = parsePseudoCode(j["Root"]["Node"])
            c += code + "\n```"

            s = {
                "prompt" : f"\"{" ".join(str(bt['instruction']).splitlines())}\"",
                "BT" : j,
                "code" : code
            }

            # s = f'''{{
            #     "prompt" : \"{" ".join(str(bt['instruction']).splitlines())}\",
            #     "BT" : '''
            # p = parseBT(bt['output'])
            # p = str(p)
            # p = p.replace('\'', '\"')
            # j = json.loads(str(p))
            # s += json.dumps(j, indent = 4)

            # s += "\n}"

            # s = json.loads(s)

            f = open(f'{path}{i}.json', 'w')
            f.write(json.dumps(s, indent = 4))
            f.close()

            i += 1
        except Exception as e:
            print(f'Error: {e} in bt: {i}\n')
            print(f'BT: {bt}\n\n\n')
            print(f'{str(s)}\n\n\n')
            print(p)
            pass

if __name__ == "__main__":
    main()