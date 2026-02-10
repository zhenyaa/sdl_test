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
        f.write("#include <SDL3/SDL.h>\n\n")
        f.write("namespace Sprites {\n\n")

        # структура Tile
        f.write("struct Tile {\n")
        f.write("    const char* textureFile;\n")
        f.write("    SDL_FRect src;\n")
        f.write("};\n\n")

        for tileset in root.findall("tileset"):
            name = tileset.attrib["name"]
            columns = int(tileset.attrib["columns"])
            tileW = int(tileset.attrib["tilewidth"])
            tileH = int(tileset.attrib["tileheight"])
            tilecount = int(tileset.attrib["tilecount"])
            image = tileset.find("image").attrib["source"]  # путь к PNG

            f.write(f"// Tileset {name} ({image})\n")
            for i in range(tilecount):
                row = i // columns
                col = i % columns
                f.write(
                    f"inline constexpr Tile {name}_{i} = {{\"{image}\", {{ {col*tileW}, {row*tileH}, {tileW}, {tileH} }}}};\n"
                )

        f.write("\n} // namespace Sprites\n")

    print(f"Header generated: {out_file}")


if __name__ == "__main__":
    tmx_file = "assets/images/TILED_files/Map.tmx"  # путь к TMX
    out_file = "generated_sprites.h"                # путь к генерируемому хедеру
    main(tmx_file, out_file)