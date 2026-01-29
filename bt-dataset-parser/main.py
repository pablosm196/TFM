from datasets import load_dataset
import json
from tqdm import tqdm
import xml.etree.ElementTree as ET

def parseBT(bt : str) -> str:
    tree = ET.parse(bt)
    root = tree.getroot()

def main():
    path = './JSONs/Example_'

    ds = load_dataset("ArtemLykov/LLM_BRAIn_dataset")

    i = 0
    for bt in tqdm(ds['train']['output']):
        s = parseBT(bt)



if __name__ == "__main__":
    main()
