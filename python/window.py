from tkinter import *
from tkinter.filedialog import askopenfilename, askdirectory
import tkinter.messagebox as tkMessageBox
from stega_w import Stegacrypto


mods = ["plain crypto", "jpeg stega", "png/bmp stega"]
submods = ["hide", "extract"]
container_types = {
    "png/bmp stega": ("png/bmp files", "*.png *.bmp"),
    "jpeg stega": ("jpeg files", "*.jpeg *.jpg"),
    "plain crypto": None
}

st_cr = Stegacrypto()

actions = {
    "plain crypto": {
        "hide": st_cr.encrypt_file,
        "extract": st_cr.decrypt_file
    },
    "jpeg stega": {
        "hide": st_cr.encode_and_hide_file_in_jpeg,
        "extract": st_cr.decode_and_take_file_from_jpeg
    },
    "png/bmp stega": {
        "hide": st_cr.encode_and_hide_file_in_bmppng,
        "extract": st_cr.decode_and_take_file_from_bmppng
    }
}


class MyFrame(Frame):
    def __init__(self):
        Frame.__init__(self)
        self.master.title("Stegacrypto")
        self.grid()


        #self.button = Button(self, text="Browse", command=self.load_file, width=10)
        #self.button.grid(row=0, column=0, sticky=E)

        self.create_files_pickers()

        self.create_pickers()
        self.create_password_section()

        self.setup_argument_picker()

        self.start_box = Button(self, text="start", command=self.start)
        self.start_box.grid(row=5, column=2, sticky=W, pady=2)

    def demo(self):
        Button(self, text="First").grid(row=0)
        Button(self, text="Second").grid(row=1)

        e1 = Entry(self)
        e2 = Entry(self)

        e1.grid(row=0, column=1)
        e2.grid(row=1, column=1)

    def create_files_pickers(self):
        self.pick_secret_button = Button(self, text="  Secret  ", command=self.set_secret)
        self.pick_secret_button.grid(row=0)
        self.secret_label = Label(self)
        self.secret_label.grid(row=0, column=2)

        self.pick_container_button = Button(self, text="Container", command=self.set_container)
        self.pick_container_button.grid(row=1)
        self.container_label = Label(self)
        self.container_label.grid(row=1, column=2)

        self.pick_output_dir_button = Button(self, text="Output dir",  command=self.set_output_dir)
        self.pick_output_dir_button.grid(row=2)
        self.output_dir_label = Label(self)
        self.output_dir_label.grid(row=2, column=2)

    def create_pickers(self):
        self.mode = IntVar()
        self.submode = IntVar()
        self.mode_pickers = []
        self.submode_pickers = []

        for (idx, mod) in enumerate(mods):
            r_b = Radiobutton(self, text=mod, variable=self.mode, value=idx, pady=5, command=self.reset_path)
            r_b.grid(row=3+idx, column=0, sticky=W)
            self.mode_pickers.append(r_b)

        for (idx, mod) in enumerate(submods):
            r_b = Radiobutton(self, text=mod, variable=self.submode, value=idx, pady=5, command=self.reset_path)
            r_b.grid(row=3+idx, column=1, sticky=W)
            self.submode_pickers.append(r_b)

    def create_password_section(self):
        self.password_label = Label(self, text="Password:")
        self.password_label.grid(row=3, column=2, sticky=W)
        self.password = StringVar()
        self.password_box = Entry(self, bg="#ffffff", width=64, textvariable=self.password)
        self.password_box.grid(row=4, column=2, sticky=W)

    def setup_argument_picker(self):
        self.arguments = {
            "plain crypto": {
                "hide": lambda: (self.secret_label['text'], self.password.get(), self.output_dir_label['text']),
                "extract": lambda: (self.secret_label['text'], self.password.get(), self.output_dir_label['text']),
            },
            "jpeg stega": {
                "hide": lambda: (self.password.get(), self.secret_label['text'],
                                 self.container_label['text'], self.output_dir_label['text'] ),
                "extract": lambda: (self.password.get(), self.container_label['text'], self.output_dir_label['text']),
            },
            "png/bmp stega": {
                "hide": lambda: (self.password.get(), self.container_label['text'], self.secret_label['text'],
                                 self.output_dir_label['text']),
                "extract": lambda: (self.password.get(), self.container_label['text'], self.output_dir_label['text']),
            }
        }

    def start(self):
        args = (self.arguments[mods[self.mode.get()]][submods[self.submode.get()]])()
        for a in args:
            if not a:
                self.alert_message("Some details are missing")
                return

        action = actions[mods[self.mode.get()]][submods[self.submode.get()]]
        try:
            action(*args)
        except Exception as e:
            self.error_message(e.__str__())

    def reset_path(self):
        self.secret_label.config(text="")
        self.output_dir_label.config(text="")
        self.container_label.config(text="")

    def set_container(self):
        filetype = container_types[mods[self.mode.get()]]
        if filetype is None:
            return

        fname = askopenfilename(filetypes=(filetype,))
        if fname:
            self.container_label.config(text=fname)

    def set_secret(self):
        fname = askopenfilename(filetypes=[("All files", "*.*")])
        if fname:
            self.secret_label.config(text=fname)

    def set_output_dir(self):
        fname = askdirectory()
        if fname:
            self.output_dir_label.config(text=fname + "/")

    def alert_message(self, message):
        tkMessageBox.showwarning("Attention", message)

    def error_message(self, message):
        tkMessageBox.showerror("Error", message)



if __name__ == "__main__":
    root = Tk()
    root.resizable(0,0)

    mf = MyFrame()
    mf.mainloop()