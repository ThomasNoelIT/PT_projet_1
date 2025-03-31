import networkx as nx
import matplotlib.pyplot as plt
import time
import os
import glob

FOLDER_NAME = "tree_snapshots"
FILE_PATTERN = os.path.join(FOLDER_NAME, "tree_*.txt")

def read_tree_from_file(filename):
    """Lit l'arbre stocké dans un fichier et renvoie un dictionnaire {pos: (delta, color, left, right)}."""
    tree = {}

    try:
        with open(filename, "r") as file:
            for line in file:
                parts = line.strip().split()
                pos, delta, color = int(parts[0]), int(parts[1]), parts[2]
                left = None
                right = None

                if len(parts) > 3:
                    for child in parts[3:]:
                        if child.startswith("L"):
                            left = int(child[1:])
                        elif child.startswith("R"):
                            right = int(child[1:])

                tree[pos] = (delta, color, left, right)
    except FileNotFoundError:
        print(f"Le fichier {filename} n'existe pas encore.")

    return tree


def draw_tree(tree):
    """Dessine l'arbre Red-Black en utilisant les vraies relations parent-enfant."""
    if not tree:
        print("Aucun nœud à afficher.")
        return

    G = nx.DiGraph()

    # Ajouter les nœuds et leurs couleurs
    for pos, (delta, color, left, right) in tree.items():
        G.add_node(pos, color=color, label=f"{pos}\nΔ{delta}")

        # Ajouter les vraies relations parent-enfant
        if left is not None:
            G.add_edge(pos, left)
        if right is not None:
            G.add_edge(pos, right)

    # Placement automatique des nœuds
    pos_map = nx.spring_layout(G)  # Positionnement basé sur les liens
    colors = ["red" if G.nodes[n]["color"] == "RED" else "black" for n in G.nodes]

    # Affichage
    plt.figure(figsize=(10, 6))
    nx.draw(G, pos=pos_map, with_labels=True, labels=nx.get_node_attributes(G, 'label'),
            node_color=colors, node_size=800, font_size=10, font_color="white", edge_color="gray")
    plt.show()


if __name__ == "__main__":
    print("Surveillance des fichiers dans", FOLDER_NAME)

    last_seen_files = set()

    while True:
        # Récupérer les fichiers dans l'ordre de création
        files = sorted(glob.glob(FILE_PATTERN), key=os.path.getmtime)

        # Vérifier les nouveaux fichiers
        new_files = [f for f in files if f not in last_seen_files]

        for filename in new_files:
            print(f"Affichage de {filename}")
            tree = read_tree_from_file(filename)
            draw_tree(tree)

            # Supprimer le fichier après l'affichage
            os.remove(filename)
            last_seen_files.add(filename)

        time.sleep(0.1)  # Pause pour éviter une surcharge CPU


#gcc -Wall -pedantic -std=c11 -O3 -o test_magic_time test_magic_time.c magic.c
#./test_magic_time | python3 visualize.py
