import json
import sys, getopt

from guidance import system, user, assistant, gen
from guidance import json as gen_json
from guidance.models import Transformers

def main():
    tasks = sys.argv[1]

    ROUTE = 'C:/Users/pablo/Desktop/Uni/Master/TFM/BTGenerator/Content/TFM/JSONs/BTschema.json'

    schema = ''
    with open(ROUTE, 'r') as f:
        schema = json.load(f)
        schema = json.dumps(schema, indent = 4)

    phi_lm = Transformers("microsoft/Phi-4-mini-instruct")

    lm = phi_lm

    with system():
        lm += f'''
You are an expert in Unreal Engine 5 and Behavior Trees (BTs).

You have to create a BT and the Blackboard (BB) from the user's description following the format given by the JSON schema.

In order to create de BB, use carefuly the info given by the user prompt (for example, if the user need a NPC that need to go to a random point, is very needed an entry in the blackboard for store that point, like "Point" : "Vector3").

You can ONLY use the following list of Tasks: {tasks}
        '''

    with user():
        lm += "NPC that, constantly, select a random point from the map, goes to the point and waits a time."

    with assistant():
        lm += gen_json(name = 'BT', schema = schema)

    loaded_json = json.loads(lm["BT"])
    print(json.dumps(loaded_json, indent=4))


if __name__ == "__main__":
    main()
