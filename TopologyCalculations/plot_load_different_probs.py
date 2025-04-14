import matplotlib
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from mpl_toolkits.axes_grid1.inset_locator import inset_axes, zoomed_inset_axes, mark_inset
matplotlib.use("TkAgg")
import numpy as np
import pickle

import os
import re
import collections


def create_pickle(directory):
    prob_dict = {}
    for p_fail in prob_list:
        if p_fail not in prob_dict:
            prob_dict[p_fail] = {}

        p_sha = 1-p_fail
        p_bin = 1-p_fail
        prob_dict[p_fail][1] = [p_bin, p_sha, 1]
    minmax = {}

    for filename in os.listdir(directory):

        fname = os.path.join(directory, filename)

        if not os.path.isfile(fname):
            print("fname")
            continue

        if not filename.endswith(".pkl"):
            continue

        print(fname)

        if not os.path.exists(fname):
            continue

        with open(fname, 'rb') as f:
            graphs = pickle.load(f) # deserialize using load()

            # print(graphs)
            split_name = fname.split('_')
            split_pickle = split_name[0].split('/')[1]
            test = re.findall(r"\d+", split_pickle)

            num_vertices = int(test[0])

            minmax[directories[jj]] = {}
            if split_name[1] == "iter":
                for p_fail in prob_list:
                    if num_vertices not in prob_dict[p_fail]:
                        prob_dict[p_fail][num_vertices] = [0, 0, 0]
                        prob_dict[p_fail][num_vertices][0] = (1-p_fail)**num_vertices

                    minmax[directories[jj]]["min"] = calculate_min(graphs)
                    minmax[directories[jj]]["max"] = calculate_max(graphs)
                    print(minmax)
                    prob_dict[p_fail][num_vertices][1] += calculate_probability_fast(graphs, p_fail, num_vertices)
                    prob_dict[p_fail][num_vertices][2] += len(graphs)

    #is there a reason this is indented? 
    with open('prob_dict'+directories[jj]+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
        pickle.dump(prob_dict, f)

    with open('minmax'+directories[jj]+'.pkl', "wb") as f: # "wb" because we want to write in binary mode
        pickle.dump(minmax, f)

    return

def calculate_min(graphs):
    m = 1000
    for g in graphs:
        if g[0] < m:
            m = g[0]
    return m

def calculate_max(graphs):
    m = 0
    for g in graphs:
        if g[0] > m:
            m = g[0]
    return m

def calculate_probability_fast(graphs, p_fail, num_vertices):
    p = 0
    for g in graphs:
        p += (p_fail**g[0])
    return p  


def process_data(prob_dict):

    for p_fail in prob_list:
        for n in prob_dict[p_fail].keys():
            if n == 1:
                # print(p_fail, n, prob_dict[p_fail][n])
                continue
            prob_dict[p_fail][n][1] = 1 - prob_dict[p_fail][n][1] / prob_dict[p_fail][n][2]

            # print(p_fail, n, prob_dict[p_fail][n])

    x = [None]*len(prob_dict[prob_list[0]].keys())
    no_share = [None]*len(prob_dict[prob_list[0]].keys())
    one_share = [None]*len(prob_dict[prob_list[0]].keys())
    two_share = [None]*len(prob_dict[prob_list[0]].keys())
    all_share = [None]*len(prob_dict[prob_list[0]].keys())

    for p_fail in prob_list:
        for i, n in enumerate(prob_dict[p_fail].keys()):
            if n == 1:
                no_share[i] = prob_dict[p_fail][n][1] ** 3
                one_share[i] = prob_dict[p_fail][n][1] ** 3
                two_share[i] = prob_dict[p_fail][n][1] ** 3
                all_share[i] = prob_dict[p_fail][n][1] ** 3
                x[i] = n
                continue    
            no_share[i] =  (prob_dict[p_fail][n][0] ** 3) * (prob_dict[p_fail][n][1] ** 0)
            one_share[i] = (prob_dict[p_fail][n][0] ** 2) * (prob_dict[p_fail][n][1] ** 1)
            two_share[i] = (prob_dict[p_fail][n][0] ** 1) * (prob_dict[p_fail][n][1] ** 2)
            all_share[i] = (prob_dict[p_fail][n][0] ** 0) * (prob_dict[p_fail][n][1] ** 3)
            x[i] = n

    no_share  = [y for _, y in sorted(zip(x, no_share))]
    one_share = [y for _, y in sorted(zip(x, one_share))]
    two_share = [y for _, y in sorted(zip(x, two_share))]
    all_share = [y for _, y in sorted(zip(x, all_share))]
    x_axis = sorted(x)

    data = [all_share, two_share, one_share, no_share, x_axis]

    return data


def plot_small_plot(data, jj):
    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))


    colors = [blues[1], reds[2], reds[1], reds[0]]
     

    fig, ax = plt.subplots(figsize=(2*2,3.5*2))
    labels = ['Sharing: 3/3','Sharing: 2/3','Sharing: 1/3','Sharing: 0/3']

    for i in range(4):
        ax.plot(data[4], np.array(data[i])*100, color=colors[i],label=labels[i], linewidth=1.5, alpha=1)
        ax.scatter(data[4], np.array(data[i])*100, color=colors[i], s=5, zorder=5)

    ax.set_ylim(0,105)
    ax.set_xlim(1,10)

    ax.set_yticks([0,50,100])
    # ax.set_xticks([1,3,5,7])

    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    # ax.tick_params(axis='both', labelcolor='none', labelsize=0)
    # ax.tick_params(axis='both', labelcolor='none', labelsize=0)

    # ax.legend(loc = 'lower left')
    # ax.set_xlabel("Number of modules")    
    # ax.set_ylabel("Probability of success") 
    fig.tight_layout()

    # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])

    plt.savefig("beeeg_numbaaahhh"+directories[jj]+".png")

    # ax.set_ylim(1,105)
    # plt.yscale("log")   

    plt.clf()
    plt.close(fig)
    return


def combine_avg_data(data_dict):
    all_data = [[] for _ in range(5)]
    all_x = []

    for i in range(3):
        if "Pickles"+str(i+1) in data_dict.keys():
            print("Pickles"+str(i+1))
            for j in range(5):
                all_data[j].append(data_dict["Pickles"+str(i+1)][j])

                # print(all_data) 
            all_x.append(data_dict["Pickles"+str(i+1)][4])
            # print(all_data)



    all_data_mean = [None]*5
    all_data_var = [None]*5
    for j in range(5):
        all_data_mean[j] = np.mean(all_data[j], axis=0)
        all_data_var[j] = np.var(all_data[j], axis=0)

    # x_axis = np.mean(all_x, axis=0)

    print()
    print("HEREEE")
    for d in all_data_mean:
        print(d)
    # print(x_axis)
    print()
    print("VAR")
    for d in all_data_var:
        print(d)

    data_dict["Average"] = all_data_mean

    return data_dict


def plot_combo_plot(data_dict):

    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))

    colors = [blues[1], reds[2], reds[1], reds[0]]
         
    # fig, ax = plt.subplots(figsize=(2*2,3.5*2))
    fig, ax = plt.subplots(figsize=(2*2,2.5*2))
    labels = ['Sharing: 3/3','Sharing: 2/3','Sharing: 1/3','Sharing: 0/3']

    for i in range(4):
        # lower_bound = np.array(all_data_mean[i]) - np.array(all_data_var[i])
        # upper_bound = np.array(all_data_mean[i]) + np.array(all_data_var[i])
        ax.plot(data_dict['Average'][4], np.array(data_dict['Average'][i])*100, color=colors[i],label=labels[i], linewidth=1.5, alpha=1)
        ax.scatter(data_dict['Average'][4], np.array(data_dict['Average'][i])*100, color=colors[i], s=20, zorder=5)
        # ax.fill_between(all_x[0], lower_bound, upper_bound, facecolor=colors[i])


    ax.set_ylim(0,105)
    ax.set_xlim(1,10)

    ax.set_yticks([0,50,100])
        # ax.set_xticks([1,3,5,7])

    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

        # ax.tick_params(axis='both', labelcolor='none', labelsize=0)
        # ax.tick_params(axis='both', labelcolor='none', labelsize=0)


        # ax.legend(loc = 'lower left')
    fig.tight_layout()

        # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])


        # plt.show() 
        # plt.savefig("probably_cool_v2.png", dpi=256)

    plt.savefig("beeeg_numbaaahhh_all_short.png")


    ax.set_xlim(1,np.max(data_dict['Average'][4]))
    ax.set_xlim(0,202)
    plt.savefig("beeeg_numbaaahhh_all_long.png", transparent=True)


    ax.set_ylim(0,105)
    plt.xscale("log")
    plt.savefig("beeeg_numbaaahhh_all_log.png", transparent=True)
    plt.clf()
    plt.close(fig)

    return


def plot_lil_dog(data_dict):
    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))


    colors = [blues[1], reds[2], reds[1], reds[0]]
     

    scale = 2
        # fig, ax = plt.subplots(figsize=(1.7*scale,1*scale))
    # fig, ax = plt.subplots(figsize=(0.8*scale,1*scale))
    fig, ax = plt.subplots(figsize=(0.6*scale,0.6*scale))
    # plt.subplots(figsize=(1.7,1))

    labels = ['Sharing: 3/3','Sharing: 2/3','Sharing: 1/3','Sharing: 0/3']

    data = np.array(data_dict['DogPickles'])[0:4]*100


    with open('minmaxDogPickles.pkl', 'rb') as f:
        minmax = pickle.load(f)['DogPickles']


    if True: #if you want error bars
        num_vertices = 10
        lower_bar = 1-((prob_list[0])**(minmax['min']))
        upper_bar = 1-((prob_list[0])**(minmax['max']))
        
        ind_fail = ((1-prob_list[0])**num_vertices)

        lower = [None]*4
        lower[0] = (lower_bar ** 3) * (ind_fail ** 0)*100
        lower[1] = (lower_bar ** 2) * (ind_fail ** 1)*100
        lower[2] = (lower_bar ** 1) * (ind_fail ** 2)*100
        lower[3] = (lower_bar ** 0) * (ind_fail ** 3)*100


        upper = [None]*4
        upper[0] = (upper_bar ** 3) * (ind_fail ** 0)*100
        upper[1] = (upper_bar ** 2) * (ind_fail ** 1)*100
        upper[2] = (upper_bar ** 1) * (ind_fail ** 2)*100
        upper[3] = (upper_bar ** 0) * (ind_fail ** 3)*100

        lower = np.array(data[:,1]) - np.array(lower) 
        upper = np.array(upper) - np.array(data[:,1])


        ax.bar([1,2,3,4], data[:,1], yerr = (lower, upper),
            color=colors,label=labels, width=0.8, linewidth=1.5, alpha=1)
    
    else:
        ax.bar([1,2,3,4], data[:,1], 
            color=colors,label=labels, linewidth=1.5, alpha=1)


    ax = formatsmallPlot(ax)
    # fig.tight_layout()

    # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])

    plt.tick_params(bottom=False)

    ax.set_yticks([0,50,100]) 
    # plt.tick_params(bottom=False, left=False) 
    
    plt.savefig("lil_puppy.png",dpi=256,transparent=True)


    plt.clf()
    plt.close(fig)
    return


def plot_wheel(data_dict):
    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))


    colors = [blues[1], reds[2], reds[1], reds[0]]
     

    fig, ax = plt.subplots(figsize=(2*2,3.5*2))
    labels = ['Sharing: 3/3','Sharing: 2/3','Sharing: 1/3','Sharing: 0/3']

    data = np.array(data_dict['WheelPickles'])[0:4]*100


    with open('minmaxWheelPickles.pkl', 'rb') as f:
        minmax = pickle.load(f)['WheelPickles']

    print(data_dict)

    if True: #if you want error bars
        num_vertices = 10
        lower_bar = 1-((prob_list[0])**(minmax['min']))
        upper_bar = 1-((prob_list[0])**(minmax['max']))
        
        ind_fail = ((1-prob_list[0])**num_vertices)

        lower = [None]*4
        lower[0] = (lower_bar ** 3) * (ind_fail ** 0)*100
        lower[1] = (lower_bar ** 2) * (ind_fail ** 1)*100
        lower[2] = (lower_bar ** 1) * (ind_fail ** 2)*100
        lower[3] = (lower_bar ** 0) * (ind_fail ** 3)*100


        upper = [None]*4
        upper[0] = (upper_bar ** 3) * (ind_fail ** 0)*100
        upper[1] = (upper_bar ** 2) * (ind_fail ** 1)*100
        upper[2] = (upper_bar ** 1) * (ind_fail ** 2)*100
        upper[3] = (upper_bar ** 0) * (ind_fail ** 3)*100

        lower = np.array(data[:,1]) - np.array(lower) 
        upper = np.array(upper) - np.array(data[:,1])


        ax.bar([1,2,3,4], data[:,1], yerr = (lower, upper),
            color=colors,label=labels, linewidth=1.5, alpha=1)
    
    else:
        ax.bar([1,2,3,4], data[:,1], 
            color=colors,label=labels, linewidth=1.5, alpha=1)


    ax = formatsmallPlot(ax)
    fig.tight_layout()

    # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])

    plt.savefig("lil_wheel.png")


    plt.clf()
    plt.close(fig)
    return



def plot_tripod(data_dict):
    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))


    colors = [blues[1], reds[2], reds[1], reds[0]]

    print("HERERE")
     

    scale = 2
        # fig, ax = plt.subplots(figsize=(1.7*scale,1*scale))
    # fig, ax = plt.subplots(figsize=(0.8*scale,1*scale))
    fig, ax = plt.subplots(figsize=(0.6*scale,0.6*scale))
    # plt.subplots(figsize=(1.7,1))

    labels = ['Sharing: 3/3','Sharing: 2/3','Sharing: 1/3','Sharing: 0/3']

    data = np.array(data_dict['TripodPickles'])[0:4]*100


    with open('minmaxTripodPickles.pkl', 'rb') as f:
        minmax = pickle.load(f)['TripodPickles']

    print(data)

    if True: #if you want error bars
        num_vertices = 4
        lower_bar = 1-((prob_list[0])**(minmax['min']))
        upper_bar = 1-((prob_list[0])**(minmax['max']))
        
        ind_fail = ((1-prob_list[0])**num_vertices)

        lower = [None]*4
        lower[0] = (lower_bar ** 3) * (ind_fail ** 0)*100
        lower[1] = (lower_bar ** 2) * (ind_fail ** 1)*100
        lower[2] = (lower_bar ** 1) * (ind_fail ** 2)*100
        lower[3] = (lower_bar ** 0) * (ind_fail ** 3)*100


        upper = [None]*4
        upper[0] = (upper_bar ** 3) * (ind_fail ** 0)*100
        upper[1] = (upper_bar ** 2) * (ind_fail ** 1)*100
        upper[2] = (upper_bar ** 1) * (ind_fail ** 2)*100
        upper[3] = (upper_bar ** 0) * (ind_fail ** 3)*100

        lower = np.array(data[:,1]) - np.array(lower) 
        upper = np.array(upper) - np.array(data[:,1])


        ax.bar([1,2,3,4], data[:,1], yerr = (lower, upper),
            color=colors,label=labels, width=0.8, linewidth=1.5, alpha=1)
    
    else:
        ax.bar([1,2,3,4], data[:,1], 
            color=colors,label=labels, linewidth=1.5, alpha=1)


    ax = formatsmallPlot(ax)
    # fig.tight_layout()

    # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])

    plt.tick_params(bottom=False)

    ax.set_yticks([0,50,100]) 
    # plt.tick_params(bottom=False, left=False) 
    
    plt.savefig("lil_tripod.png",dpi=256,transparent=True)


    plt.clf()
    plt.close(fig)
    return


def formatsmallPlot(ax):
    # ax.tick_params(axis='both', labelcolor='none', labelsize=0)
    # ax.tick_params(axis='both', labelcolor='none', labelsize=0)

    # ax.legend(loc = 'lower left')
    # ax.set_xlabel("Number of modules")    
    # ax.set_ylabel("Probability of success")

    scale = 3
    # plt.rcParams['figure.figsize'] = [0.6*scale,0.3*scale]   
    fig = matplotlib.pyplot.gcf()
    fig.set_size_inches(0.25*scale,0.5*scale)    
    ax.set_ylim(0,105)
    fig.set_dpi(256)

    ax.set_yticks([0,50,100])

    ax.tick_params(axis='both', labelcolor='none', labelsize=0)
    # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])    

    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    return ax


def plot_all_options(data_dict):
    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))

    colors = [blues[1], reds[2], reds[1], reds[0]]
         
    fig, ax = plt.subplots(figsize=(2*2,3.5*2))
    plot_keys = ['CirclePickles', 'Average','LinePickles']
    labels = ['Best', 'All/Random', 'Worst']

    for i, ky in enumerate(plot_keys):
        ax.plot(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i],label=labels[i], linewidth=2.0, alpha=1)
        # ax.plot(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i],label=labels[i], linewidth=1.5, alpha=1)
        ax.scatter(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i], s=40, zorder=5)
        # ax.scatter(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i], s=5, zorder=5)
        # ax.fill_between(all_x[0], lower_bound, upper_bound, facecolor=colors[i])

    ax.set_ylim(85,101)
    ax.set_xlim(0,205)

    ax.set_xticks([0,50,100,150,200])
    ax.set_yticks([85,90, 95, 100])

    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    ax.set_xlabel("Number of modules")    
    ax.set_ylabel("Reliability (%)") 

    # ax.tick_params(axis='both', labelcolor='none', labelsize=0)

    leg = ax.legend(loc = 'lower left', fontsize=12, frameon=False) 
    # set the linewidth of each legend object
    for legobj in leg.legend_handles:
        legobj.set_linewidth(3)    
    
    fig.tight_layout()

    # plt.gca().set_position([0.05, 0.05, 0.93, 0.95])


    # axins = inset_axes(ax, width='75%', height='40%', loc='center')

    axins = inset_axes(
        ax,
        width="85%",
        height="50%",
        bbox_to_anchor=(0.20, 0.4, 0.90, 0.7),  # anchor in Axes coords (slightly below center)
        # bbox_to_anchor=(0.2, 0.5, 0.75, 0.5),  # anchor in Axes coords (slightly below center)
        bbox_transform=ax.transAxes,
        loc="lower center"           # align the lower edge of the inset with the anchor
    )

    # axins.set_box_aspect(1)

    # Plot the same data on the inset
    for i, ky in enumerate(plot_keys):
        axins.plot(
            data_dict[ky][4],
            np.array(data_dict[ky][0]) * 100,
            color=colors[i],
            linewidth=1.5,
            alpha=1
        )
        axins.scatter(
            data_dict[ky][4],
            np.array(data_dict[ky][0]) * 100,
            color=colors[i],
            s=15,
            zorder=5
        )

    # Set the “zoom” range
    axins.set_xlim(1, 10.5)     # x-range for zoom
    axins.set_ylim(99, 100.1)  # y-range for zoom

    axins.set_yticks([99,100])
    axins.set_xticks([1,5,9])

    # Optionally, remove or reduce tick labels on the inset:
    axins.tick_params(axis='both', which='major', labelsize=8)

    axins.spines["top"].set_visible(False)
    axins.spines["right"].set_visible(False)
    axins.spines["left"].set_visible(False)
    axins.spines["bottom"].set_visible(False)

    # axins.tick_params(axis='both', labelcolor='none', labelsize=0)

    # Draw lines showing where the inset fits in the larger plot
    # mark_inset returns (patch, connector_line1, connector_line2)
    patch, c1, c2 = mark_inset(
        ax, axins,
        loc1=2,         # which corner(s) to connect, e.g., upper-left
        loc2=4,         # lower-right
        fc="none",      # no fill
        ec="black",     # edge color of the bounding rectangle
        lw=1            # optional: line width
    )

    # Now manually set the connector line colors to gray
    c1.set_color("gray")
    c2.set_color("gray")

    # print(plt.gcf())
    # plt.show()
    # exit()
    plt.savefig("all_three.png", transparent=True)

    return




def plot_all_options_v2(data_dict):
    blues = matplotlib.colormaps.get_cmap('Blues')
    blues = blues(np.linspace(1,0.5,4))

    reds = matplotlib.colormaps.get_cmap('Reds')
    reds = reds(np.linspace(0.9,0.4,3))

    colors = ["xkcd:forest green", "xkcd:gold", "xkcd:crimson"]
    colors = ["xkcd:crimson", "xkcd:gold", "xkcd:forest green"]
         
    # fig, ax = plt.subplots(figsize=(2*2,3.5*2))
    scale = 1
    fig, ax = plt.subplots(figsize=(3*scale,1.5*scale))

    # plot_keys = ['CirclePickles', 'Average','LinePickles']
    # labels = ['Best', 'All/Random', 'Worst']
    labels = ['Worst', 'All/Random', 'Best']
    plot_keys = ['LinePickles', 'Average','CirclePickles']

    for i, ky in enumerate(plot_keys):
        ax.plot(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i],label=labels[i], linewidth=1.5, alpha=1)
        # ax.plot(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i],label=labels[i], linewidth=1.5, alpha=1)
        ax.scatter(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i], s=8, zorder=5)
        # ax.scatter(data_dict[ky][4], np.array(data_dict[ky][0])*100, color=colors[i], s=5, zorder=5)
        # ax.fill_between(all_x[0], lower_bound, upper_bound, facecolor=colors[i])


    rect = patches.Rectangle((1, 99), 10.5, 1, linewidth=2, edgecolor='xkcd:red', facecolor='none')
    ax.add_patch(rect)

    ax.set_ylim(80,102)
    ax.set_xlim(0,205)

    ax.set_xticks([0,50,100,150,200])
    ax.set_yticks([80,90, 100])

    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    # ax.set_xlabel("Number of modules")    
    # ax.set_ylabel("Reliability (%)") 

    ax.tick_params(axis='both', labelcolor='none', labelsize=0)

    # leg = ax.legend(loc = 'lower left', fontsize=12, frameon=False) 
    # # set the linewidth of each legend object
    # for legobj in leg.legend_handles:
    #     legobj.set_linewidth(3)    
    
    fig.tight_layout()

    plt.savefig("all_three_v2_beeg.png", transparent=True, dpi=256)
    plt.clf()
    plt.close(fig)


    scale = 2
    fig, axins = plt.subplots(figsize=(0.8*scale,0.8*scale))
    # fig, axins = plt.subplots(figsize=(2*2,3.5*2))

    # axins = inset_axes(
    #     ax,
    #     width="85%",
    #     height="50%",
    #     bbox_to_anchor=(0.20, 0.4, 0.90, 0.7),  # anchor in Axes coords (slightly below center)
    #     # bbox_to_anchor=(0.2, 0.5, 0.75, 0.5),  # anchor in Axes coords (slightly below center)
    #     bbox_transform=ax.transAxes,
    #     loc="lower center"           # align the lower edge of the inset with the anchor
    # )

    # axins.set_box_aspect(1)

    # Plot the same data on the inset
    for i, ky in enumerate(plot_keys):
        axins.plot(
            data_dict[ky][4],
            np.array(data_dict[ky][0]) * 100,
            color=colors[i],
            linewidth=2,
            alpha=1
        )
        axins.scatter(
            data_dict[ky][4],
            np.array(data_dict[ky][0]) * 100,
            color=colors[i],
            s=20,
            zorder=5
        )

    # Set the “zoom” range
    axins.set_xlim(1, 10.5)     # x-range for zoom
    axins.set_ylim(99, 100.1)  # y-range for zoom

    axins.set_yticks([99,100])
    axins.set_xticks([1,10])

    # Optionally, remove or reduce tick labels on the inset:
    axins.tick_params(axis='both', which='major', labelsize=8)

    # axins.spines["top"].set_visible(False)
    # axins.spines["right"].set_visible(False)

    axins.tick_params(axis='both', labelcolor='none', labelsize=0)


    # axins.tick_params(axis='both', labelcolor='none', labelsize=0)

    # Draw lines showing where the inset fits in the larger plot
    # mark_inset returns (patch, connector_line1, connector_line2)
    # patch, c1, c2 = mark_inset(
    #     ax, axins,
    #     loc1=2,         # which corner(s) to connect, e.g., upper-left
    #     loc2=4,         # lower-right
    #     fc="none",      # no fill
    #     ec="black",     # edge color of the bounding rectangle
    #     lw=1            # optional: line width
    # )

    # Now manually set the connector line colors to gray
    # c1.set_color("gray")
    # c2.set_color("gray")

    # print(plt.gcf())
    # plt.show()
    # exit()
    plt.savefig("all_three_v3.png", transparent=True, dpi=256)

    return



# prob_list = [0.05, 0.25, 0.50, 0.75]
# prob_list = [0.1]
# prob_list = [0.25]
prob_list = [0.05]
# for i in range(1,8):
# for i in range(1,5):


directories = ['Pickles1']
directories = ['Pickles1', 'Pickles2']
# directories = ['Pickles1', 'Pickles2', 'Pickles3']
# directories = ['LinePickles']
# directories = ['CirclePickles']
directories = ['Pickles1', 'Pickles2', 'Pickles3', 'LinePickles', 'CirclePickles', 'DogPickles', 'WheelPickles']
# directories = ['DogPickles', 'WheelPickles']

directories = ['Pickles1', 'Pickles2', 'Pickles3', 'LinePickles', 
   'CirclePickles', 'DogPickles', 'WheelPickles', 'TripodPickles']


data_dict = {}

for jj, directory in enumerate(directories):
    
    #Change to true to reload, in case of changing prob_list
    # if True:
    if False:
        create_pickle(directory)

    prob_dict = None
    with open('prob_dict'+directories[jj]+'.pkl', 'rb') as f:
        prob_dict = pickle.load(f)

    data = process_data(prob_dict)
    plot_small_plot(data, jj)

    # data_dict[directories[jj]] = data[0]
    data_dict[directories[jj]] = data

# exit()

data_dict = combine_avg_data(data_dict)

# plot_combo_plot(data_dict)

# plot_all_options(data_dict)
# plot_all_options_v2(data_dict)

plot_lil_dog(data_dict)
# plot_wheel(data_dict)
plot_tripod(data_dict)


exit()
