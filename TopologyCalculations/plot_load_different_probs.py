import matplotlib
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")
import numpy as np
import pickle

import os



def calculate_probability(graphs, p_fail, num_vertices):
    p = 0
    for g in graphs:
        num = g.min_okay
        p += (p_fail**num)
    p = p / len(graphs) # Same as dividing within the sum
    p = 1-p
    
    return p



def calculate_probability_tmp(graphs, p_fail, num_vertices):
    p = 0
    for g in graphs:
        num = g.min_okay
        p += (p_fail**num)
    return p    


prob_dict = {}
# prob_list = [0.05, 0.25, 0.50, 0.75]
# prob_list = [0.1]
# prob_list = [0.25]
prob_list = [0.05]
# for i in range(1,8):
# for i in range(1,5):

    
for p_fail in prob_list:
    if p_fail not in prob_dict:
        prob_dict[p_fail] = {}

    p_sha = 1-p_fail
    p_bin = 1-p_fail
    prob_dict[p_fail][1] = [p_bin, p_sha, 1]




directory = 'Pickles2'

#Change to true to reload, in case of changing prob_list
# if True:
if False:

    # iterate over files in that directory
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

            split_name = fname.split('_')

            num_vertices = int(split_name[0][-1])

            if split_name[1] == "iter":
                for p_fail in prob_list:
                    if num_vertices not in prob_dict[p_fail]:
                        prob_dict[p_fail][num_vertices] = [0, 0, 0]
                        prob_dict[p_fail][num_vertices][0] = (1-p_fail)**num_vertices

                    
                    prob_dict[p_fail][num_vertices][1] += calculate_probability_tmp(graphs, p_fail, num_vertices)
                    prob_dict[p_fail][num_vertices][2] += len(graphs)


    with open('prob_dict.pkl', "wb") as f: # "wb" because we want to write in binary mode
        pickle.dump(prob_dict, f)


prob_dict = None
with open('prob_dict.pkl', 'rb') as f:
    prob_dict = pickle.load(f)


# print(prob_dict)

for p_fail in prob_list:
    for n in prob_dict[p_fail].keys():
        if n == 1:
            print(p_fail, n, prob_dict[p_fail][n])
            continue
        prob_dict[p_fail][n][1] = 1 - prob_dict[p_fail][n][1] / prob_dict[p_fail][n][2]

        print(p_fail, n, prob_dict[p_fail][n])



no_share = [None]*len(prob_dict[prob_list[0]].keys())
one_share = [None]*len(prob_dict[prob_list[0]].keys())
two_share = [None]*len(prob_dict[prob_list[0]].keys())
all_share = [None]*len(prob_dict[prob_list[0]].keys())
for p_fail in prob_list:
    for n in prob_dict[p_fail].keys():
        if n == 1:
            no_share[0] = prob_dict[p_fail][n][1] ** 3
            one_share[0] = prob_dict[p_fail][n][1] ** 3
            two_share[0] = prob_dict[p_fail][n][1] ** 3
            all_share[0] = prob_dict[p_fail][n][1] ** 3
            continue        
        no_share[n-1] =  (prob_dict[p_fail][n][0] ** 3) * (prob_dict[p_fail][n][1] ** 0)
        one_share[n-1] = (prob_dict[p_fail][n][0] ** 2) * (prob_dict[p_fail][n][1] ** 1)
        two_share[n-1] = (prob_dict[p_fail][n][0] ** 1) * (prob_dict[p_fail][n][1] ** 2)
        all_share[n-1] = (prob_dict[p_fail][n][0] ** 0) * (prob_dict[p_fail][n][1] ** 3)

        print((prob_dict[p_fail][n][0] ** 0), (prob_dict[p_fail][n][1] ** 0))


print("HERE")
print(no_share)
print(one_share)
print(two_share)
print(all_share)


blues = matplotlib.cm.get_cmap('Blues')
blues = blues(np.linspace(1,0.5,4))

reds = matplotlib.cm.get_cmap('Reds')
reds = reds(np.linspace(0.9,0.4,3))


colors = [blues[1], reds[2], reds[1], reds[0]]
 

# fig, ax = plt.subplots(figsize=(2,3.5))
# fig, ax = plt.subplots(figsize=(1,1.7))
fig, ax = plt.subplots(figsize=(1.7,1))

x = list(range(1, len(prob_dict[p_fail].keys())+1))

# plt.axhline(no_share[0]*100, color = 'xkcd:gray', linestyle = '--')
ax.plot(x, np.array(all_share)*100, color=colors[0],label='Sharing: 3/3', linewidth=1, alpha=0.5)
ax.plot(x, np.array(two_share)*100, color=colors[1],label='Sharing: 2/3', linewidth=1, alpha=0.5)
ax.plot(x, np.array(one_share)*100, color=colors[2],label='Sharing: 1/3', linewidth=1, alpha=0.5)
ax.plot(x, np.array(no_share)*100, color=colors[3],label='Sharing: 0/3', linewidth=1, alpha=0.5)

# plt.axhline(100, color = 'xkcd:gray', linestyle = '-', zorder=0)

ax.scatter(x, np.array(all_share)*100, color=colors[0], s=5, zorder=5)
ax.scatter(x, np.array(two_share)*100, color=colors[1], s=5, zorder=5)
ax.scatter(x, np.array(one_share)*100, color=colors[2], s=5, zorder=5)
ax.scatter(x, np.array(no_share)*100, color=colors[3], s=5, zorder=5)


            # ax.scatter(x, np.array(data_s)*100, color=reds[p], marker='o', s=5,zorder=5)

ax.set_ylim(0,105)
ax.set_xlim(1,len(x)+0.2)

ax.set_yticks([0,50,100])
ax.set_xticks([1,3,5,7])

ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)

ax.tick_params(axis='both', labelcolor='none', labelsize=0)


# ax.legend(loc = 'lower left')
# ax.set_xlabel("Number of modules")    
# ax.set_ylabel("Probability of success") 
fig.tight_layout()

plt.gca().set_position([0.05, 0.05, 0.93, 0.95])


plt.show()
# plt.savefig("probably_cool_v2.png", dpi=256)

plt.clf()



exit()
print("noot")


fig, ax = plt.subplots()

x = list(range(1, len(prob_dict[p_fail].keys())+1))

for p_fail in prob_list:
    prob = prob_dict[p_fail]
    data_s = [prob[n][1] for n in prob.keys()]
    data_i = [prob[n][0] for n in prob.keys()]
    plt.axhline(data_s[0]*100, color = 'xkcd:gray', linestyle = '--')
    ax.plot(x, np.array(data_s)*100, color='xkcd:scarlet')
    ax.plot(x, np.array(data_i)*100, color='xkcd:blue')

plt.axhline(100, color = 'xkcd:gray', linestyle = '-', zorder=0)


ax.set_ylim(0,105)


ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)

ax.set_xlabel("Number of modules")    
ax.set_ylabel("Probability of success")    

plt.savefig("probably_cool_v2.png")

plt.clf()

fig, ax = plt.subplots(ncols=2, sharey=True)


for p_fail in prob_list:
    prob = prob_dict[p_fail]
    data_s = [prob[n][1] for n in prob.keys()]
    data_i = [prob[n][0] for n in prob.keys()]
    # plt.axhline(data_s[0]*100, color = 'xkcd:gray', linestyle = '--')
    ax[0].plot(x, np.array(data_i)*100, color='xkcd:scarlet')
    ax[1].plot(x, np.array(data_s)*100, color='xkcd:blue')
    # ax[0].axhline(data_s[0]*100, color = 'xkcd:gray', linestyle = '--')
    # ax[1].axhline(data_s[0]*100, color = 'xkcd:gray', linestyle = '--')

for i in range(2):
    ax[i].axhline(100, color = 'xkcd:gray', linestyle = '-', zorder=0)
    ax[i].set_ylim(0,100)
    ax[i].set_xlim(1,len(x))

    ax[i].spines["top"].set_visible(False)
    ax[i].spines["right"].set_visible(False)

ax[0].set_xlabel("Number of modules")    
ax[1].set_xlabel("Number of modules")    
ax[0].set_ylabel("Probability of success")    

ax[0].set_yticks([0,25,50,75,100])

# fig.subplots_adjust(wspace=0)


# plt.savefig("75.png")
# plt.savefig("95.png")
# plt.savefig("99.png")
plt.savefig("probably_coolerer.png",dpi=128)


plt.clf()

blues = matplotlib.cm.get_cmap('Blues')
blues = blues(np.linspace(1,0.5,4))

reds = matplotlib.cm.get_cmap('Reds')
reds = reds(np.linspace(0.9,0.4,4))

for f in range(2):

    fig, ax = plt.subplots(figsize=(1,1.7))
    # plt.figsize()


    for p, p_fail in enumerate(prob_list):
        prob = prob_dict[p_fail]
        if f == 0:
            data_s = [prob[n][0] for n in prob.keys()]
            # ax.plot(x, np.array(data_s)*100, color=reds[p], marker='o', markersize=2, linewidth=1)
            ax.plot(x, np.array(data_s)*100, color=reds[p], linewidth=1, alpha=0.5)
            ax.scatter(x, np.array(data_s)*100, color=reds[p], marker='o', s=5,zorder=5)
        else:
            data_i = [prob[n][1] for n in prob.keys()]
            # ax.plot(x, np.array(data_i)*100, color=blues[p], marker='o', markersize=2, linewidth=1)
            ax.plot(x, np.array(data_i)*100, color=blues[p], linewidth=1, alpha=0.5)
            ax.scatter(x, np.array(data_i)*100, color=blues[p], marker='o', s=5,zorder=5)


    for i in range(2):
        ax.set_ylim(0,105)
        ax.set_xlim(1,len(x)+0.2)

        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)

    ax.set_yticks([0,50,100])
    ax.set_xticks([1,3,5,7])

    ax.tick_params(axis='both', labelcolor='none', labelsize=0)
    # plt.tick_params(left=False)
    # ax.spines["left"].set_visible(False)
    fig.tight_layout()

    plt.gca().set_position([0.05, 0.05, 0.93, 0.95])

    plt.savefig("indiv_"+str(f)+"_noot.png",dpi=256)
    plt.clf()





# plt.show()