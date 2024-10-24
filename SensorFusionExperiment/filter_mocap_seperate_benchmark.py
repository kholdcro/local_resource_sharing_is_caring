import time
import sys
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib.collections as collections
import matplotlib.gridspec as gridspec
from mpl_toolkits.axes_grid1.inset_locator import inset_axes, mark_inset
# from mpl_toolkits.axes_grid1.inset_locator import zoomed_inset_axes

colors =    ['xkcd:scarlet',
            'xkcd:green',
            'xkcd:blue'
            ]

error_colors = ['xkcd:light red',
                'xkcd:light green',
                'xkcd:light blue'
                ]


colordict = {
    'raw': 'xkcd:scarlet',
    'sf': 'xkcd:green',
    'dis': 'xkcd:blue'
}

RY = 15
BY = 3

DEG = 5

def load_data(num, filename):
    # data = pd.read_csv('vicon_data/2023-04-27-{}-{}.csv'.format(num, filename), skiprows=[0,1,2,3,4])
    # name = 'vicon_data/2023-06-02-{} {}.csv'.format(num, filename)
    name = 'vicon_data/2023-06-09-{}-{}.csv'.format(num, filename)
    print(name)
    data = pd.read_csv(name, skiprows=[0,1,2,3])
    data.columns = data.columns.str.replace(' ', '')
    return np.array(data)


def load_datalog(num, filename):
    # data = pd.read_csv('vicon_data/2023-04-27-{}-{}.csv'.format(num, filename), skiprows=[0,1,2,3,4])
    # name = 'mori_datalog/{}_{}.csv'.format(num, filename)
    name = 'mori_datalog/{}-{}.csv'.format(num, filename)
    print(name)
    data = pd.read_csv(name, skiprows=[0])
    data = np.array(data)
    #Bob is the one which disables its sensors, on the base
    cond = np.array(data[:,0] == "000AC172")
    base = data[cond,:]

    cond = np.array(data[:,0] == "00D00BD2")
    lift = data[cond,:]
    return base, lift
    

def normalize_time(data):
    arr = -np.degrees(data[:,RY])
    # print(np.abs(arr-arr[0]))
    idx = np.where(np.abs(arr-arr[10])>DEG)[0][0]

    tmp = np.flip(arr)

    idx_back = np.where(np.abs(tmp-tmp[10])>DEG)[0][0]   
    idx_back = len(tmp) - idx_back
    # base_raw = base_raw[:, :idx]

    data[:,0] = (data[:,0] - data[idx,0]) / 100 
    return data[idx:idx_back,:]


def normalize_time_ind(data, COL):
    # Bob is connected to edge 3, Gina edge 2, so we need to adjust in CSVs
    if data[0,0] == "000AC172":
        COL += 1

    arr = np.array(data[:,COL])
    cond = arr > 0
    arr = arr[cond]
    idx = np.where(np.abs(arr-arr[10])>DEG)[0][0]
    tmp = data[cond,:]
    refined_data = np.array([tmp[:,1] - tmp[idx,1], tmp[:,COL]])     
    refined_data = refined_data[:, idx:]

    tmp = np.flip(refined_data[1,:])

    idx = np.where(np.abs(tmp-tmp[10])>DEG)[0][0]   
    idx = len(tmp) - idx
    refined_data = refined_data[:, :idx]

    refined_data[0,:] = refined_data[0,:] / 1000 
    refined_data[1,:] = 180 - refined_data[1,:]

    return refined_data


def normalize_time_both(gt_time, gt_data, angle):
    time1 = np.array(gt_time, dtype='float64')
    data1 = np.array(gt_data, dtype='float64')

    time2 = np.array(angle[0,:], dtype='float64')
    data2 = np.array(angle[1,:], dtype='float64')

    start_time = min(np.min(time1), np.min(time2))
    end_time = max(np.max(time1), np.max(time2))


    common_time = np.linspace(start_time, end_time, num=len(time1)+len(time2))

    aligned_data1 = np.interp(common_time, time1, data1)
    aligned_data2 = np.interp(common_time, time2, data2)

    aligned_data1 = (aligned_data1-np.min(aligned_data1))/(np.max(aligned_data1)-np.min(aligned_data1))
    aligned_data2 = (aligned_data2-np.min(aligned_data2))/(np.max(aligned_data2)-np.min(aligned_data2))


    aligned_data1 = 2*aligned_data1 - 1
    aligned_data2 = 2*aligned_data2 - 1
    
    cross_corr = np.correlate(aligned_data1, aligned_data2, mode='full')
    # print(cross_corr)
    time_shift = np.argmax(cross_corr) - (len(aligned_data2) - 1)

    time_step =  (end_time - start_time) / (len(time1)+len(time2) -1)
    angle[0,:] = time2 + time_shift * time_step

    return angle


def get_error(gt_time, gt_data, angle):
    error = 0
    var_all = 0
    N = angle.shape[1]

    time1 = np.array(gt_time, dtype='float64')
    data1 = np.array(gt_data, dtype='float64')

    time2 = np.array(angle[0,:], dtype='float64')
    data2 = np.array(angle[1,:], dtype='float64')

    start_time = min(np.min(time1), np.min(time2))
    end_time = max(np.max(time1), np.max(time2))

    common_time = np.linspace(start_time, end_time, num=len(time1)+len(time2))

    aligned_data1 = np.interp(common_time, time1, data1)
    aligned_data2 = np.interp(common_time, time2, data2)    

    error = np.abs(aligned_data1 - aligned_data2)
    mean_error = np.mean(error)
    var_all = np.std(error)

    return mean_error, var_all, (common_time, error)



def normalize_raw(base, lift):
    time1 = np.array(base[0,:], dtype='float64')
    data1 = np.array(base[1,:], dtype='float64')

    time2 = np.array(lift[0,:], dtype='float64')
    data2 = np.array(lift[1,:], dtype='float64')

    start_time = min(np.min(time1), np.min(time2))
    end_time = max(np.max(time1), np.max(time2))

    common_time = np.linspace(start_time, end_time, num=len(time1)+len(time2))

    aligned_data1 = np.interp(common_time, time1, data1)
    aligned_data2 = np.interp(common_time, time2, data2) 

    tmp = np.array(aligned_data1 + aligned_data2) / 2
    raw_value = np.vstack([common_time, tmp])
    # raw_value = np.concatenate(, axis=1)

    return raw_value


def main():
    start = time.time()
    
    filename  = 'dis'
    filetypes = ['raw', 'sf', 'dis']
    filedescript = ['Average', 'Sensor Fusion', 'Dead Module']
    # filetypes = ['sf']

    filenums = [3, 4, 5, 6]

    # filetypes = ['dis']
    # filenums = [3]

    # fig, axs = plt.subplots(nrows=1, ncols=1)

    RAW = 15
    SF = 3

    SHOW_RAW = False
    # SHOW_RAW = True

    fig_heights = [4.5, 3.3]
    fig, axs = plt.subplots(nrows=2, sharex=True, gridspec_kw={'height_ratios': fig_heights, 'hspace':0.1})

    for jj in range(2):
        print(jj)
        err_mean = []
        err_var = []
        high_means = []
        high_vars = []
        low_means = []
        low_vars = []

        bench_arr_mean = []
        bench_arr_var = []

        plotlist  = []
        x_scatter = []

        colorlist = []

        for i, file in enumerate(filetypes):
            for num in filenums:
                colorlist.append(colors[i])
                x_scatter.append(num)
                plotlist.append(str(num))

                data = load_data(num, file)
                base, lift = load_datalog(num, file)

                if file == 'dis' and num == 6:
                    base = base[:len(base)-500,:]

                data = normalize_time(data)
                filter_time = data[:,0]


                base_raw = normalize_time_ind(base, RAW)
                base_sf  = normalize_time_ind(base, SF)
                
                lift_raw = normalize_time_ind(lift, RAW)
                lift_sf  = normalize_time_ind(lift, SF)

                # Are we sure this is the correct thing?
                # ground_truth = -np.degrees(data[:,RY])+np.degrees(data[:,BY])
                ground_truth = -np.degrees(data[:,RY])-np.degrees(data[:,BY])
                

                if jj == 0:
                    raw_value = base_raw
                    sf_value = base_sf
                else:
                    raw_value = lift_raw
                    sf_value = lift_sf

                # raw_value = normalize_raw(base_raw, lift_raw)

                # _, _, err_plot_lift_sf = get_error(filter_time, ground_truth, lift_sf)


                if file == "raw":


                    if SHOW_RAW == True:
                        raw_value = normalize_time_both(filter_time, ground_truth, raw_value)
                        mean_all, var_all, _ = get_error(filter_time, ground_truth, raw_value)
                    else:
                        norm_raw = normalize_raw(base_raw, lift_raw)
                        norm_raw = normalize_time_both(filter_time, ground_truth, norm_raw)
                        mean_all, var_all, bench_error = get_error(filter_time, ground_truth, norm_raw)

                else:
                    norm_sf = normalize_time_both(filter_time, ground_truth, sf_value)
                    mean_all, var_all, _ = get_error(filter_time, ground_truth, norm_sf)

                if SHOW_RAW == False:
                    if not file == "dis" or jj == 1:
                        benchmark = normalize_time_both(filter_time, ground_truth, raw_value)
                        bench_mean, bench_var, _ = get_error(filter_time, ground_truth, benchmark)
                        
                        bench_arr_mean.append(bench_mean)
                        bench_arr_var.append(bench_var)

                    # benchmark = normalize_raw(base_raw, lift_raw)
                    # benchmark = normalize_time_both(filter_time, ground_truth, benchmark)
                    # bench_mean, bench_var, bench_error = get_error(filter_time, ground_truth, benchmark)

                err_mean.append(mean_all)
                err_var.append(var_all)


        # print(x_scatter)
        # print(err_mean)
        # axs.plot(x_scatter, err_mean, color=colorlist)

        for i in [0,1,2]:
            slc_low = i*4
            slc_high = 4+i*4
            slc = np.s_[slc_low:slc_high]
            # axs.errorbar(x_scatter[slc], err_mean[slc], yerr=err_var[slc], 
            #     color=colors[i], linewidth=4, 
            #     ecolor=colors[i], elinewidth=2,
            #     label=filedescript[i], solid_capstyle='round')

            # axs.errorbar(x_scatter[slc], high_means[slc], yerr=high_vars[slc], 
            #     color=colors[i], linewidth=4, 
            #     ecolor=colors[i], elinewidth=2,
            #     label=filedescript[i]+"high")

            # axs.-rrorbar(x_scatter[slc], low_means[slc], yerr=low_vars[slc], 
            #     color=colors[i], linewidth=4, 
            #     ecolor=colors[i], elinewidth=2,
            #     label=filedescript[i]+"low", alpha=0.7)


        # axs.bar(plotlist, err_mean, yerr=err_var, 
        #     color=colorlist, linewidth=1, 
        #     # ecolor=colors[i], 
        #     label=plotlist)





        M = 4
        x = np.arange(4)  # the label locations
        width = 0.25  # the width of the bars
        multiplier = 0

        # fig, axs = plt.subplots([1, 2], layout='constrained')

        print(bench_arr_mean)
        for i in range(3):
            slc_low = i*M
            slc_high = M+i*M
            slc = np.s_[slc_low:slc_high]
            offset = width * multiplier
            # print(x, offset, err_mean[i], width, slc)
            axs[jj].bar(x + offset, err_mean[slc], width,
                yerr=err_var[slc], 
                label=filedescript[i], color=colorlist[slc],
                )
            # Plot benchmark average
            # if i == 0:
                # axs[jj].bar(x + offset, bench_arr_mean, width, label=None, edgecolor='black', color='none')
            # axs[jj].bar(x+offset+width/2, bench_arr_mean[slc], width/4, label=None, color='xkcd:gold')
            if SHOW_RAW == False:
                if i < 2 or jj ==1:
                    print(x, bench_arr_mean[slc])
                    axs[jj].scatter(x+offset-0.005, bench_arr_mean[slc], marker='D', color='black', s=10,
                        label='Angle Meas.' if i == 0 else None)

            # axs[jj].bar_label(rects, padding=3)
            multiplier += 1
            # print(filedescript[i],i,err_mean)
        # ax.set_xticks(x + width, species)
        

        # axs[jj].bar(plotlist, err_mean, yerr=err_var, color=colorlist)



        # axs[jj].set_ylim(0,3.85)
        axs[jj].set_ylim(0, fig_heights[jj])
        # axs[jj].set_ylim(0,)

        if jj == 0:
            ncol = 1 
            if SHOW_RAW == False:
                ncol = 2

            leg = axs[jj].legend(loc='upper left', ncol=ncol, fontsize=12, markerscale=1, frameon=False, framealpha=0.5)

            for legobj in leg.legend_handles:
                print(legobj)
                legobj.set_linewidth(4.0)


        # axs[jj].set_xticks([0.5, 1.5, 2.5, 3.5])
        axs[jj].set_yticks([0, 2])
        if jj == 0:
            axs[jj].set_yticks([0, 2, 4])


        axs[jj].spines["top"].set_visible(False)
        axs[jj].spines["right"].set_visible(False)
        axs[jj].tick_params(axis='both', which='major', labelsize=12)

        # plt.ylabel("Error (deg)", fontsize=14)
        axs[jj].set_ylabel("Module {}".format(jj), fontsize=14)

        # plt.ylabel()
        # plt.xlabel("Number of Modules", fontsize=14)
        axs[jj].tick_params(
        axis='x',          # changes apply to the x-axis
        which='both',      # both major and minor ticks are affected
        bottom=False,      # ticks along the bottom edge are off
        labelbottom=False) # labels along the bottom edge are off



    # plt.show()
    # input("Press [enter] to continue.")

    # plt. tight_layout()

    # plt.savefig("error_lifting.png",dpi=256)
    plt.savefig("error_of_two_modules.png",dpi=128)
    plt.savefig("error_of_two_modules.png",dpi=256)
    # plt.savefig("error_lifting.png",dpi=64)

if __name__ == '__main__':
    main()