import os

from termcolor import colored
import tkinter as tk
import json
import time
from tkinter import filedialog
from collections import OrderedDict

from Settings import names


class LoadFile():

    def __init__(self, frame, wifi_host):
        self.frame = frame
        self.wifi_host = wifi_host
        self.fileName = []
        self.fileContents = []
        self.iteration = 0
        self.auto_update_flag = False
        self.auto_update_run = False
        self.auto_delay_flag = False
        self.save_stable_time = False
        self.auto_delay_time = 0.
        self.load()

    def load(self):

        frame_1 = tk.Frame(self.frame)
        frame_2 = tk.Frame(self.frame)
        frame_3 = tk.Frame(self.frame)

        frame_auto_update = tk.Frame(frame_1)
        frame_load_display = tk.Frame(frame_1)
        frame_disp_text = tk.Frame(frame_1)
        frame_prev_next = tk.Frame(frame_3)

        self.auto_update_button = tk.Button(frame_auto_update)
        self.auto_update_button["text"] = "Manual"
        self.auto_update_button["command"] = lambda: self.toggleAutoUpdate()
        self.auto_update_button.pack({"side": "bottom"})

        self.auto_update_label = tk.Label(frame_auto_update, text="File Progression")
        self.auto_update_label.pack({"side": "top"})             

        load_button = tk.Button(frame_load_display)
        load_button["text"] = "LoadFile",
        load_button["command"] = lambda: self.openFile()
        load_button.pack({"side": "top"},expand=True)

        self.next_stage = tk.Label(frame_load_display, font='Helvetica 10 bold')
        self.next_stage["text"] = "No File Loaded"
        self.next_stage.pack({"side": "bottom"}, fill=tk.X, expand=True)

        self.show_json = tk.Label(frame_disp_text, anchor='w')
        self.show_json["text"] = ""
        self.show_json.pack({"side": "top"}, fill=tk.X)

        self.prev_button = tk.Button(frame_prev_next)
        self.prev_button["text"] = "Prev",
        self.prev_button["command"] = lambda: self.prevJson()
        self.prev_button.pack({"side": "left"}, fill=tk.X, expand=True)

        self.next_button = tk.Button(frame_prev_next)
        self.next_button["text"] = "Next",
        self.next_button["command"] = lambda: self.iterateJson()
        self.next_button.pack({"side": "left"}, fill=tk.X, expand=True)

        self.run_button = tk.Button(frame_prev_next)
        self.run_button["text"] = "Run",
        self.run_button["fg"]   = "green"
        self.run_button["command"] = lambda: self.runButtonClicked()
        self.run_button.pack({"side": "right"}, fill=tk.X, expand=True)  

        frame_auto_update.pack(side="top", expand=True, pady=10)
        frame_load_display.pack(side="top", expand=True)
        frame_disp_text.pack(side="top", fill=tk.X, expand=True)

        frame_prev_next.pack(side="bottom", expand=True)

        frame_3.pack(side="top", pady=10)
        frame_1.pack(side="top", expand=True, fill=tk.X)
        frame_2.pack(side="top", expand=True, fill=tk.X)


    def openFile(self):
        if not os.path.exists("CommandFiles"):
            os.makedirs("CommandFiles")
        ftypes = [('JSON', '*.json'), ('All files', '*')]
        self.fileName = filedialog.askopenfilename(initialdir ="CommandFiles", filetypes=ftypes)

        try:
            with open(self.fileName, 'r') as f:
                self.fileContents = json.load(f, object_pairs_hook=self.value_resolver)
        except FileNotFoundError:
            print("Cannot open file")
            return
        self.iteration = 0
        self.displayJson()

    def displayJson(self):
        try:
            self.next_stage["text"] = self.fileContents[self.iteration]["Label"][0]
        except KeyError:
            self.next_stage["text"] = ""
        textstr = ""
        for module in self.fileContents[self.iteration]:
            if module == 'Label':
                continue
            for cmd in self.fileContents[self.iteration][module]:
                textstr = textstr + module[:4] + ": " + cmd + "\n"
        self.show_json["text"] = textstr[:-1] #remove last \n


    def iterateJson(self):
        if self.iteration < len(self.fileContents)-1:
            self.iteration += 1
            self.displayJson()
            self.prev_button["fg"] = "black"
        else:
            self.next_button["fg"] = "gray"
            if self.auto_update_flag == True:
                self.toggleAutoUpdate()


    def prevJson(self):
        if self.iteration > 0:
            self.iteration -= 1
            self.displayJson()
            self.next_button["fg"] = "black"
            if self.iteration <= 0:
                self.prev_button["fg"] = "gray"
        else:
            self.prev_button["fg"] = "gray"    

    def runButtonClicked(self):
        if self.auto_update_flag == True:
            self.auto_update_run = not self.auto_update_run
            self.toggleRunButton()
        else:
            self.run_button["text"] = "Run",
            self.run_button["fg"]   = "green"            
            self.runJson()        

    def runJson(self):
        if 'Jump' in self.fileContents[self.iteration]:
            self.jumpToSection()
        if 'Delay'in self.fileContents[self.iteration]:
            self.setAutoDelay()
        for module in self.fileContents[self.iteration]:
            if module.lower() == 'label':
                continue
            if module.lower() == 'delay':
                continue                
            if module.lower() == "global":
                for cmd in self.fileContents[self.iteration][module]:
                    print("Global: ", cmd)
                    self.wifi_host.publishGlobal(cmd)
                continue
            for cmd in self.fileContents[self.iteration][module]:
                if cmd.startswith("ping"):
                    num = 1
                    splitText = cmd.split("ping",1)
                    if (splitText[1].strip().isnumeric()):
                        num = int(splitText[1])
                    self.wifi_host.pingHandler.setPingCount(names.checkName(module), num)
                    continue
                self.wifi_host.publishLocal(cmd, names.checkName(module))  
        self.iterateJson()


    def value_resolver(self, pairs):
        unique_modules = list(OrderedDict.fromkeys([i[0] for i in pairs]))
        new_dict = {}
        for module in unique_modules:
            values = [v for k, v in pairs if k == module]
            new_dict[module] = values
        return dict(new_dict)          


    def toggleAutoUpdate(self):
        if self.auto_update_button['text'] == "Manual":
            self.auto_update_flag = True
            self.auto_update_run = False
            self.run_button["text"] = "Run",
            self.run_button["fg"]   = "green"             
            self.last_update_time = time.time()
            self.auto_update_button.configure(text="Automatic", fg="green")
        else:
            self.auto_update_flag = False
            self.auto_update_button.configure(text="Manual", fg="red")

    def toggleRunButton(self):
        if self.auto_update_run == False:
            self.run_button["text"] = "Stop",
            self.run_button["fg"]   = "red"
        else:
            self.run_button["text"] = "Run",
            self.run_button["fg"]   = "green"


    def checkAutoUpdate(self):
        if (self.auto_update_flag == False) or (self.auto_update_run == False):
            return

        if (time.time() - self.last_update_time < 0.25):
            return


        stable_state = self.wifi_host.getStableState()
        if (self.save_stable_time == True):
            self.stable_time = time.time()
            self.save_stable_time = False
        if all(stable_state[module] == True for module in stable_state):
            if (self.auto_delay_flag == True):
                if (time.time() - self.stable_time < self.auto_delay_time):
                    return
            self.auto_delay_flag == False
            self.runJson()
            self.last_update_time = time.time()


    def jumpToSection(self):
        section_jump = self.fileContents[self.iteration]['Jump'][0]
        for i, cmdList in enumerate(self.fileContents):
            if section_jump in cmdList['Label']:
                self.iteration = i
                self.displayJson()
                return
        print(colored('GUI Error: No label found corresponding to the jump', 'red'))
        self.iterateJson()

    def setAutoDelay(self):
        if (self.save_stable_time == True): 
            return
        self.auto_delay_flag = True
        self.save_stable_time = True
        print(self.fileContents[self.iteration]['Delay'][0])
        self.auto_delay_time = float(self.fileContents[self.iteration]['Delay'][0])
        return