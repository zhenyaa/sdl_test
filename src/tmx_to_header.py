import sys
import xml.etree.ElementTree as ET
import os

def main(tmx_file: str, out_file: str):
    if not os.path.exists(tmx_file):
        print(f"Error: TMX file '{tmx_file}' not found")
        return

    tree = ET.parse(tmx_file)
    root = tree.getroot()

    with open(out_file, "w") as f:
        f.write(f"// Generated from {tmx_file}\n")
        f.write("#pragma once\n\n")
        f.write("namespace Sprites {\n")

        for tileset in root.findall("tileset"):
            name = tileset.attrib["name"]
            firstgid = int(tileset.attrib["firstgid"])
            columns = int(tileset.attrib["columns"])
            tileW = int(tileset.attrib["tilewidth"])
            tileH = int(tileset.attrib["tileheight"])
            f.write(f"// Tileset {name}\n")
            for i in range(int(tileset.attrib["tilecount"])):
                row = i // columns
                col = i % columns
                f.write(f"inline constexpr SDL_FRect {name}_{i} = {{ {col*tileW}, {row*tileH}, {tileW}, {tileH} }};\n")

        f.write("}\n")
    print(f"Header generated: {out_file}")


if __name__ == "__main__":
    # Можно менять пути под свои файлы
    tmx_file = "assets/images/TILED_files/Map.tmx"
    out_file = "generated_sprites.h"
    main(tmx_file, out_file)