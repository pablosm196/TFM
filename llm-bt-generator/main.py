import json

from guidance import system, user, assistant, gen
from guidance import json as gen_json
from guidance.models import Transformers

def main():

    ROUTE = 'C:/Users/pablo/Desktop/Uni/Master/TFM/BTGenerator/Content/TFM/JSONs/BTschema.json'

    schema = ''
    with open(ROUTE, 'r') as f:
        schema = json.load(f)
        schema = json.dumps(schema, indent = 4)

    phi_lm = Transformers("microsoft/Phi-4-mini-instruct")

    lm = phi_lm

    with system():
        lm += "You are an expert in Unreal Engine 5 and Behavior Trees (BTs). You have to create a BT from the user's description following the format given by the JSON schema."

    with user():
        lm += "NPC that, constantly, select a random point from the map, goes to the point and waits a time."

    with assistant():
        lm += gen_json(name = 'BT', schema = schema)

    loaded_json = json.loads(lm["BT"])
    print(json.dumps(loaded_json, indent=4))


if __name__ == "__main__":
    main()
