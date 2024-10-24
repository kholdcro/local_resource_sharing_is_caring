import networkx as nx
import itertools
import matplotlib
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")
import numpy as np
from numba import jit

import pickle
import time

from iteration_utilities import random_combination


def generate_connected_graphs(start, num_vertices):
    vertices = list(range(0, num_vertices))
    edges = list(itertools.combinations(vertices, 2))
    print(edges)

    graphs = []
    counter = 0
    for num_edges in range(num_vertices-1, num_vertices * (num_vertices-1) // 2 + 1):
        for subset in itertools.combinations(edges, num_edges):

            graphs, counter = process_subset(subset, num_vertices, graphs, counter, start)

    with open('Pickles2/graphs'+str(num_vertices)+'_iter_'+str(counter)+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
        pickle.dump(graphs, f)
    return graphs


# # Working function with multiprocessing
# @jit(nopython=True)
# @jit()
def process_subset(subset, num_vertices, graphs, counter, start):
    g = nx.Graph(subset)
    if g.order() < num_vertices:
        return graphs, counter
    if not nx.is_connected(g):
        return graphs, counter
    # degree_sequence = sorted([d for n, d in g.degree()], reverse=True)
    degree_sequence = np.sort([d for n, d in g.degree()][::-1])
    if any(deg > 3 for deg in degree_sequence):
        return graphs, counter
    g = calculate_failure_modes(g)
    graphs.append(g)
    counter += 1

    iterator = 10000
    if counter > 2500000:
        iterator = 1000
    elif counter > 5000000:
        iterator = 500


    if len(graphs) % iterator == 0:
        print("Iter: {}, Interval: {:.2f}".format(counter, (time.time()-start)))
        with open('Pickles2/graphs'+str(num_vertices)+'_iter_'+str(counter)+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
            pickle.dump(graphs, f)
        del graphs[:]

    return graphs, counter


    # Since we calculated all permutations in the graphs themselves. 
    # When the function iterates to the next graph, it will have the same topology with the a new layout of nodes.
    # Therefore setting a single node (e.g., label 0) to fail has the same effect of sequentually calculating a failed node across all topologies
    # If we increase the number of failures, we can use the next value to describe a failure, since it will have the same effect of sequentially calculating two failed nodes across a fixed topology (e.g., 0 and 1 as failed)
    # Since the numbers are ordered, we can use their increment to calculate if they care connected to a failed node. If we assume that failures happen from 0 to N, then by increasing the number, we produce all potential failures for a topology.
    # Therefore, we can consider a node as unsupported if it is only connected to nodes who have a lower value than itself.
    # If we count down from the highest value, we can see the highest number of supported failures. We can assume all values below it are okay.
    # Extracting this value, we can say the successes are the total number of modules - number of failured modules
def calculate_failure_modes(graph):
    for v in range(graph.order()):
        g_tmp = check_graph_max(graph, v)
        if g_tmp is not None:
            break
    return g_tmp


def check_graph_max(g, v):
    for j in range(v+1):
        n_sorted = np.argsort(g.nodes())
        if all(con <= v for con in g.neighbors(j)):
            g.min_okay = v + 1 
            return g
    return None



if __name__ == '__main__':
    start = time.time()
    p_fail = 0.05

    # for num_vertices in range(2,8):
    for num_vertices in [8]:
        graphs = generate_connected_graphs(start, num_vertices)

    print("Program finished in {:.2} seconds".format(time.time()-start))