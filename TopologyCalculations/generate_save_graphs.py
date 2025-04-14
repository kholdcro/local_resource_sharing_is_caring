import random
import networkx as nx
import numpy as np
import pickle

import matplotlib
import matplotlib.pyplot as plt

def check_graph_max(g, v):
    """
    For j in [0..v], check if ALL neighbors of j have label <= v.
    If that holds for *some* j in [0..v], set g.min_okay = v+1 and return g.
    Otherwise return None.
    """
    # The line 'n_sorted = np.argsort(g.nodes())' was in your original code,
    # but not actually used in the check. We keep it for clarity, 
    # though it's not essential for the logic.
    n_sorted = np.argsort(g.nodes())

    for j in range(v + 1):
        # Condition: all(con <= v for con in g.neighbors(j))
        if all(neighbor <= v for neighbor in g.neighbors(j)):
            g.min_okay = v + 1
            return g
    return None


def calculate_failure_modes(graph):
    """
    Iterate over v in [0..graph.order()-1], calling check_graph_max.
    If check_graph_max returns a graph, we break and return that graph 
    (which then has the attribute 'min_okay').
    If we exhaust all v, we return None.
    """
    for v in range(graph.order()):
        g_tmp = check_graph_max(graph, v)
        if g_tmp is not None:
            return g_tmp
    return None


def generate_random_connected_graphs_ensured_with_min_okay(n,
                                                           max_degree=3,
                                                           max_count=100,
                                                           max_attempts=10000,
                                                           add_extra_edges=True):
    """
    Randomly generate up to `max_count` unique connected graphs on n vertices,
    each with no vertex having degree > max_degree, and also compute 'min_okay'.

    Steps:
      1) Build a random spanning tree (guarantees connectivity).
      2) Optionally add extra edges (still ensuring deg <= max_degree).
      3) Call calculate_failure_modes(g). If it returns None, discard that graph;
         otherwise, keep it (it now has attribute g.min_okay).
      4) Store the final graph as a canonical tuple of edges, so we don't get duplicates.
    """
    all_graphs = set()
    attempts = 0

    while len(all_graphs) < max_count and attempts < max_attempts:
        g = _build_random_connected_graph(n, max_degree, add_extra_edges)
        if g is not None:
            # incorporate the min_okay logic
            g_with_min_okay = calculate_failure_modes(g)
            if g_with_min_okay is not None:
                # Construct a canonical representation (sorted tuple of edges).
                # We also keep an extra record for min_okay if desired—here just in the graph object.
                edges_tuple = tuple(sorted(tuple(sorted(e)) for e in g_with_min_okay.edges()))
                edges_tuple = tuple((g.min_okay, edges_tuple))
                # add to the set
                all_graphs.add(edges_tuple)
        attempts += 1

    # Return the unique edge sets in sorted order (by size, then lex)
    return sorted(all_graphs, key=lambda e: (len(e), e))


def _build_random_connected_graph(n, max_degree=3, add_extra_edges=True):
    """
    Build a single random connected graph (or return None if we fail).
    1) Create a random spanning tree by picking an order of nodes and connecting
       each new node to exactly one existing node with deg < max_degree.
    2) Optionally, add more edges randomly if neither endpoint is at max_degree.
    """
    g = nx.Graph()
    g.add_nodes_from(range(n))
    degrees = [0]*n

    # Random order in which we "activate" nodes
    node_order = list(range(n))
    random.shuffle(node_order)

    # Place the first node, no edges
    connected_set = [node_order[0]]

    for i in range(1, n):
        new_node = node_order[i]
        # Among the connected nodes, pick one with deg < max_degree
        candidates = [u for u in connected_set if degrees[u] < max_degree]
        if not candidates:
            # no way to attach this node without exceeding max_degree
            return None
        chosen_parent = random.choice(candidates)
        # add edge
        g.add_edge(chosen_parent, new_node)
        degrees[chosen_parent] += 1
        degrees[new_node] += 1
        connected_set.append(new_node)

    # We now have a spanning tree (connected, n-1 edges).
    # Need to re-address; would be better to have n random edges and randomly add them
    # Forces the graph to have a connectivity average of ~2 which is not accurate
    if add_extra_edges:
        # Attempt to add random extra edges. For demonstration, let's pick edges
        # in random order and add them with 50% probability if both endpoints < max_degree.
        possible_edges = []
        for u in range(n):
            for v in range(u+1, n):
                if not g.has_edge(u, v):
                    possible_edges.append((u, v))
        random.shuffle(possible_edges)

        # Choose a random number K of edges to add
        K = random.randint(0, len(possible_edges))
        # print(g.edges(), possible_edges)

        # Try adding edges up to K times, checking degree constraints
        added_count = 0
        for (u, v) in possible_edges:
            if added_count >= K:
                break
            if degrees[u] < max_degree and degrees[v] < max_degree:
                g.add_edge(u, v)
                degrees[u] += 1
                degrees[v] += 1
                added_count += 1
                # print("HERE")

    return g


def generate_line_graphs(n,
                                                           max_degree=3,
                                                           max_count=100,
                                                           max_attempts=10000,
                                                           add_extra_edges=True):
    """
    Randomly generate up to `max_count` unique connected graphs on n vertices,
    each with no vertex having degree > max_degree, and also compute 'min_okay'.

    Steps:
      1) Build a random spanning tree (guarantees connectivity).
      2) Optionally add extra edges (still ensuring deg <= max_degree).
      3) Call calculate_failure_modes(g). If it returns None, discard that graph;
         otherwise, keep it (it now has attribute g.min_okay).
      4) Store the final graph as a canonical tuple of edges, so we don't get duplicates.
    """
    all_graphs = set()
    attempts = 0

    while len(all_graphs) < max_count and attempts < max_attempts:
        g = _build_line_graph(n, max_degree, add_extra_edges)
        if g is not None:
            # incorporate the min_okay logic
            g_with_min_okay = calculate_failure_modes(g)
            if g_with_min_okay is not None:
                # Construct a canonical representation (sorted tuple of edges).
                # We also keep an extra record for min_okay if desired—here just in the graph object.
                edges_tuple = tuple(sorted(tuple(sorted(e)) for e in g_with_min_okay.edges()))
                edges_tuple = tuple((g.min_okay, edges_tuple))
                # add to the set
                all_graphs.add(edges_tuple)
        attempts += 1

    # Return the unique edge sets in sorted order (by size, then lex)
    return sorted(all_graphs, key=lambda e: (len(e), e))


def _build_line_graph(n, max_degree=3, add_extra_edges=True):
    """
    Build a single line graph (or return None if we fail).
    1) Create a random spanning tree by picking an order of nodes and connecting
       each new node to exactly one existing node with deg < max_degree.
    2) Optionally, add more edges randomly if neither endpoint is at max_degree.
    """

    #generates a line / path graph with n nodes, but we want to shuffle the failure order
    #g = nx.path_graph(n)

    g = nx.Graph()
    g.add_nodes_from(range(n))
    degrees = [0]*n

    # Random order in which we "activate" nodes
    node_order = list(range(n))
    random.shuffle(node_order)

    for i in range(len(node_order)-1):
        g.add_edge(node_order[i], node_order[i+1])


    # nx.draw(g, with_labels = True)
    # plt.show()        

    return g



def generate_circle_graphs(n,
                                                           max_degree=3,
                                                           max_count=100,
                                                           max_attempts=10000,
                                                           add_extra_edges=True):
    """
    Randomly generate up to `max_count` unique connected graphs on n vertices,
    each with no vertex having degree > max_degree, and also compute 'min_okay'.

    Steps:
      1) Build a random spanning tree (guarantees connectivity).
      2) Optionally add extra edges (still ensuring deg <= max_degree).
      3) Call calculate_failure_modes(g). If it returns None, discard that graph;
         otherwise, keep it (it now has attribute g.min_okay).
      4) Store the final graph as a canonical tuple of edges, so we don't get duplicates.
    """
    all_graphs = set()
    attempts = 0

    while len(all_graphs) < max_count and attempts < max_attempts:
        g = _build_circle_graph(n, max_degree, add_extra_edges)
        if g is not None:
            # incorporate the min_okay logic
            g_with_min_okay = calculate_failure_modes(g)
            if g_with_min_okay is not None:
                # Construct a canonical representation (sorted tuple of edges).
                # We also keep an extra record for min_okay if desired—here just in the graph object.
                edges_tuple = tuple(sorted(tuple(sorted(e)) for e in g_with_min_okay.edges()))
                edges_tuple = tuple((g.min_okay, edges_tuple))
                # add to the set
                all_graphs.add(edges_tuple)
        attempts += 1

    # Return the unique edge sets in sorted order (by size, then lex)
    return sorted(all_graphs, key=lambda e: (len(e), e))


def _build_circle_graph(n, max_degree=3, add_extra_edges=True):
    """
    Build a single random connected graph (or return None if we fail).
    1) Create a random spanning tree by picking an order of nodes and connecting
       each new node to exactly one existing node with deg < max_degree.
    2) Optionally, add more edges randomly if neither endpoint is at max_degree.
    """
    g = nx.Graph()
    g.add_nodes_from(range(n))
    degrees = [0]*n

    # Random order in which we "activate" nodes
    node_order = list(range(n))
    random.shuffle(node_order)

    # Place the first node, no edges
    connected_set = [node_order[0]]

    for i in range(1, n):
        new_node = node_order[i]
        # Among the connected nodes, pick one with deg < max_degree
        candidates = [u for u in connected_set if degrees[u] < max_degree]
        if not candidates:
            # no way to attach this node without exceeding max_degree
            return None
        chosen_parent = random.choice(candidates)
        # add edge
        g.add_edge(chosen_parent, new_node)
        degrees[chosen_parent] += 1
        degrees[new_node] += 1
        connected_set.append(new_node)

    # We now have a spanning tree (connected, n-1 edges).
    # Need to re-address; would be better to have n random edges and randomly add them
    # Forces the graph to have a connectivity average of ~2 which is not accurate
    if add_extra_edges:
        # Attempt to add random extra edges. For demonstration, let's pick edges
        # in random order and add them with 50% probability if both endpoints < max_degree.
        possible_edges = []
        for u in range(n):
            for v in range(u+1, n):
                if not g.has_edge(u, v):
                    possible_edges.append((u, v))
        random.shuffle(possible_edges)


        # Try adding edges up to K times, checking degree constraints
        added_count = 0
        for (u, v) in possible_edges:
            if degrees[u] < max_degree and degrees[v] < max_degree:
                g.add_edge(u, v)
                degrees[u] += 1
                degrees[v] += 1
                added_count += 1

    return g



def generate_dog_graph(n,
                                                           max_degree=3,
                                                           max_count=100,
                                                           max_attempts=10000,
                                                           add_extra_edges=True):

    all_graphs = set()
    attempts = 0

    while len(all_graphs) < max_count and attempts < max_attempts:
        g = _build_dog_graph()
        if g is not None:
            # incorporate the min_okay logic
            g_with_min_okay = calculate_failure_modes(g)
            if g_with_min_okay is not None:
                # Construct a canonical representation (sorted tuple of edges).
                # We also keep an extra record for min_okay if desired—here just in the graph object.
                edges_tuple = tuple(sorted(tuple(sorted(e)) for e in g_with_min_okay.edges()))
                edges_tuple = tuple((g.min_okay, edges_tuple))
                # add to the set
                all_graphs.add(edges_tuple)

                # nx.draw(g, with_labels = True, node_color=color)
                # plt.savefig('wheelgraphs/'+str(len(all_graphs)))
                # plt.clf()
                # plt.close()

        attempts += 1

    # Return the unique edge sets in sorted order (by size, then lex)
    return sorted(all_graphs, key=lambda e: (len(e), e))


def _build_dog_graph():
    n = 10
    g = nx.Graph()
    g.add_nodes_from(range(10))

    # Random order in which we "activate" nodes
    node_order = list(range(n))
    random.shuffle(node_order)

    # Place the first node, no edges
    connected_set = [node_order[0]]

    g.add_edge(node_order[0], node_order[1]) #hip
    for i in range(2):
        g.add_edge(node_order[0], node_order[2+i*4]) #hip
        g.add_edge(node_order[2+i*4], node_order[3+i*4]) #foot

        g.add_edge(node_order[1], node_order[4+i*4]) #hip
        g.add_edge(node_order[4+i*4], node_order[5+i*4]) #foot

    return g


def generate_wheel_graphs(n,
                                                           max_degree=3,
                                                           max_count=100,
                                                           max_attempts=10000,
                                                           add_extra_edges=True):
    """
    Randomly generate up to `max_count` unique connected graphs on n vertices,
    each with no vertex having degree > max_degree, and also compute 'min_okay'.

    Steps:
      1) Build a random spanning tree (guarantees connectivity).
      2) Optionally add extra edges (still ensuring deg <= max_degree).
      3) Call calculate_failure_modes(g). If it returns None, discard that graph;
         otherwise, keep it (it now has attribute g.min_okay).
      4) Store the final graph as a canonical tuple of edges, so we don't get duplicates.
    """
    all_graphs = set()
    attempts = 0

    color = matplotlib.colormaps.get_cmap('coolwarm')
    color = color(np.linspace(0,1,n))

    while len(all_graphs) < max_count and attempts < max_attempts:
        g = _build_wheel_graph(n, max_degree, add_extra_edges)
        if g is not None:
            # incorporate the min_okay logic
            g_with_min_okay = calculate_failure_modes(g)
            if g_with_min_okay is not None:
                # Construct a canonical representation (sorted tuple of edges).
                # We also keep an extra record for min_okay if desired—here just in the graph object.
                edges_tuple = tuple(sorted(tuple(sorted(e)) for e in g_with_min_okay.edges()))
                edges_tuple = tuple((g.min_okay, edges_tuple))
                # add to the set
                all_graphs.add(edges_tuple)
            # color_map = [None]*n
            # for i, node in enumerate(g):
            #     print(i, node)
            # pos = nx.circular_layout(g)
            # nx.draw(g, with_labels = True, node_color=color)
            # plt.savefig('wheelgraphs/'+str(len(all_graphs)))
            # plt.clf()
            # plt.close()
        attempts += 1

    # Return the unique edge sets in sorted order (by size, then lex)
    return sorted(all_graphs, key=lambda e: (len(e), e))


def _build_wheel_graph(n, max_degree=3, add_extra_edges=True):
    """
    Build a single line graph (or return None if we fail).
    1) Create a random spanning tree by picking an order of nodes and connecting
       each new node to exactly one existing node with deg < max_degree.
    2) Optionally, add more edges randomly if neither endpoint is at max_degree.
    """

    #generates a line / path graph with n nodes, but we want to shuffle the failure order
    #g = nx.path_graph(n)

    g = nx.Graph()
    g.add_nodes_from(range(n))
    degrees = [0]*n

    # Random order in which we "activate" nodes
    node_order = list(range(n))
    random.shuffle(node_order)

    for i in range(len(node_order)-1):
        g.add_edge(node_order[i], node_order[i+1])

    g.add_edge(node_order[0], node_order[-1])

    # nx.draw(g, with_labels = True)
    # plt.show()

    return g



def generate_tripod_graph(n,
                                                           max_degree=3,
                                                           max_count=100,
                                                           max_attempts=10000,
                                                           add_extra_edges=True):

    all_graphs = set()
    attempts = 0

    while len(all_graphs) < max_count and attempts < max_attempts:
        g = _build_tripod_graph()
        if g is not None:
            # incorporate the min_okay logic
            g_with_min_okay = calculate_failure_modes(g)
            if g_with_min_okay is not None:
                # Construct a canonical representation (sorted tuple of edges).
                # We also keep an extra record for min_okay if desired—here just in the graph object.
                edges_tuple = tuple(sorted(tuple(sorted(e)) for e in g_with_min_okay.edges()))
                edges_tuple = tuple((g.min_okay, edges_tuple))
                # add to the set
                all_graphs.add(edges_tuple)

                # nx.draw(g, with_labels = True)
                # plt.savefig('wheelgraphs/'+str(len(all_graphs)))
                # plt.show()
                # plt.clf()
                # plt.close()

        attempts += 1

    # Return the unique edge sets in sorted order (by size, then lex)
    return sorted(all_graphs, key=lambda e: (len(e), e))


def _build_tripod_graph():
    n = 4
    g = nx.Graph()
    g.add_nodes_from(range(n))

    # Random order in which we "activate" nodes
    node_order = list(range(n))
    random.shuffle(node_order)

    # Place the first node, no edges
    connected_set = [node_order[0]]

    for i in range(3):
        g.add_edge(node_order[0], node_order[i+1]) #hip
    
    return g


# ----------------------
# Usage example
# ----------------------
if __name__ == "__main__":
    import time

    start_time = time.time()

    # Let's try for n=30 with a modest number of results
    # so we can show we do indeed get some connected graphs with deg <=3
    n = 30
    for n in [2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 100, 150,200]:
    # for n in [2, 3]:
    # for n in [5]:
        max_degree = 3
        max_count = 10000    # How many unique graphs we want
        # max_count = 10    # How many unique graphs we want
        max_attempts = 50000  # We'll do up to 10k random tries

        #Set to true to search all possible random graphs
        if False:
            for i in range(3):
                tmp_time = time.time()
                graphs = generate_random_connected_graphs_ensured_with_min_okay(n, max_degree,
                                                                   max_count, max_attempts,
                                                                   add_extra_edges=True)
                print(f"Found {len(graphs)} unique connected graphs (deg <= {max_degree}) for n={n}.")
                # for edges in results:
                    # print(f"Num edges={len(edges)}, edges={edges}")
                with open('Pickles'+str(i+1)+'/graphs'+str(n)+'_iter_'+str(len(graphs))+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
                    pickle.dump(graphs, f)
                print(f"Time = {time.time()-tmp_time:.2f}s")


        # Set to true to get all possible graphs in a line
        if False:
            tmp_time = time.time()
            graphs = generate_line_graphs(n, max_degree,
                                                               max_count, max_attempts,
                                                               add_extra_edges=True)
            print(f"Found {len(graphs)} unique connected graphs (deg <= {max_degree}) for n={n}.")
            # print(graphs)
            # for edges in results:
                # print(f"Num edges={len(edges)}, edges={edges}")
            with open('LinePickles/graphs'+str(n)+'_iter_'+str(len(graphs))+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
                pickle.dump(graphs, f)
            print(f"Time = {time.time()-tmp_time:.2f}s")


        # Set to true to get all possible graphs in a line
        if False:
            tmp_time = time.time()
            graphs = generate_circle_graphs(n, max_degree,
                                                               max_count, max_attempts,
                                                               add_extra_edges=True)
            print(f"Found {len(graphs)} unique connected graphs (deg <= {max_degree}) for n={n}.")
            # print(graphs)
            # for edges in results:
                # print(f"Num edges={len(edges)}, edges={edges}")
            with open('CirclePickles/graphs'+str(n)+'_iter_'+str(len(graphs))+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
                pickle.dump(graphs, f)
            print(f"Time = {time.time()-tmp_time:.2f}s")                


    n = 10
    # Set to true to get all possible graphs in a shape of a dog
    # if True:
    if False:
        tmp_time = time.time()
        graphs = generate_dog_graph(10, max_count = max_count, max_attempts = max_attempts)
        print(f"Found {len(graphs)} unique connected graphs (deg <= {max_degree}) for n={10}.")
        # print(graphs)
        # for edges in results:
            # print(f"Num edges={len(edges)}, edges={edges}")
        with open('DogPickles/graphs'+str(n)+'_iter_'+str(len(graphs))+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
            pickle.dump(graphs, f)
        print(f"Time = {time.time()-tmp_time:.2f}s")      


    # Set to true to get all possible graphs in a wheel
    if False:
        tmp_time = time.time()
        graphs = generate_wheel_graphs(10, max_count = max_count, max_attempts = max_attempts)
        print(f"Found {len(graphs)} unique connected graphs (deg <= {max_degree}) for n={10}.")
        # print(graphs)
        # for edges in results:
            # print(f"Num edges={len(edges)}, edges={edges}")
        with open('WheelPickles/graphs'+str(n)+'_iter_'+str(len(graphs))+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
            pickle.dump(graphs, f)
        print(f"Time = {time.time()-tmp_time:.2f}s")     


    n = 4
    # if True:
    if True:
        tmp_time = time.time()
        graphs = generate_tripod_graph(10, max_count = max_count, max_attempts = max_attempts)
        print(f"Found {len(graphs)} unique connected graphs (deg <= {max_degree}) for n={10}.")
        # print(graphs)
        # for edges in results:
            # print(f"Num edges={len(edges)}, edges={edges}")
        with open('TripodPickles/graphs'+str(n)+'_iter_'+str(len(graphs))+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
            pickle.dump(graphs, f)
        print(f"Time = {time.time()-tmp_time:.2f}s") 


    print(f"\nTime = {time.time()-start_time:.2f}s")
