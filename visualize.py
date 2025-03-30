import networkx as nx
import matplotlib.pyplot as plt
import time
import os
import glob

FOLDER_NAME = "tree_snapshots"
FILE_PATTERN = os.path.join(FOLDER_NAME, "tree_*.txt")

def read_tree_from_file(filename):
    """Lit l'arbre stocké dans un fichier et renvoie une liste de tuples (pos, delta, color)."""
    tree = []
    try:
        with open(filename, "r") as file:
            lines = file.readlines()
            for line in lines:
                parts = line.strip().split()
                if len(parts) == 3:
                    pos, delta, color = int(parts[0]), int(parts[1]), parts[2]
                    tree.append((pos, delta, color))
    except FileNotFoundError:
        print(f"Le fichier {filename} n'existe pas encore.")
    return tree

def draw_tree(tree):
    """Dessine l'arbre Red-Black en utilisant NetworkX et Matplotlib."""
    if not tree:
        print("Aucun nœud à afficher.")
        return
    
    G = nx.DiGraph()

    # Ajouter les nœuds avec leurs couleurs
    for pos, delta, color in tree:
        G.add_node(pos, color=color, label=f"{pos}\nΔ{delta}")

    # Simuler des relations entre nœuds (à adapter si besoin)
    sorted_nodes = sorted(tree, key=lambda x: x[0])  # Trier par pos
    parent_map = {}  # Dictionnaire pour lier parent/enfant

    for i, (pos, delta, color) in enumerate(sorted_nodes):
        if i == 0:
            parent_map[pos] = None  # La racine n'a pas de parent
        else:
            parent = sorted_nodes[(i - 1) // 2][0]  # Simulation parent/enfant
            parent_map[pos] = parent
            G.add_edge(parent, pos)

    # Définir les positions et couleurs des nœuds
    pos_map = {node: (node, -depth) for depth, (node, _, _) in enumerate(tree)}
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
