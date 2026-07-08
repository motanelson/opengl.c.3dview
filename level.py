import os 
import copy
import tkinter as tk
from tkinter import font
from PIL import Image, ImageTk
import random
import subprocess
class tApp:
    def __init__(self, root,texts,titles,yyy,xxx):
        self.root = root
        self.root.title(titles)
        self.root.geometry("800x600")
        self.root.configure(bg="white")

        # Frame com barra de scroll
        self.frame = tk.Frame(self.root, bg="white")
        self.frame.pack(fill="both", expand=True)
        self.frame2 = tk.Frame(self.root, bg="white")
        self.frame2.pack(fill="x", expand=False)

        # Canvas para desenhar texto
        self.canvas = tk.Canvas(self.frame, bg="white", highlightthickness=0)
        self.canvas.pack(side="left", fill="both", expand=True)

        # Barra de scroll vertical
        self.scrollbar = tk.Scrollbar(self.frame, orient="vertical", command=self.canvas.yview)
        self.scrollbar.pack(side="right", fill="y")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)
        self.scrollbarx = tk.Scrollbar(self.frame2, orient="horizontal", command=self.canvas.xview)
        self.scrollbarx.pack(side="bottom", fill="x")
        self.canvas.configure(xscrollcommand=self.scrollbarx.set,height=35)

        # Fonte
        self.font = font.Font(family="Courier", size=16)
        xx=0
        yy=0
        rrr=100
        y=20
        self.images = {}
        for t in "0123":
            img = Image.open(t + ".bmp")
            self.images[t] = ImageTk.PhotoImage(img)
        for t in texts:
            
            
            if t=="\n":
                yy=yy+rrr
                xx=0
            else:
                tt=self.canvas.create_image(xx, yy,image=self.images[t])
                xx=xx+rrr
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        # Scroll automático para o fim
        self.yyy=(yyy+1)*rrr
        self.xxx=(xxx+1)*rrr
        print(self.xxx)
        print(self.yyy)
        self.canvas.yview_moveto(0.0)
        self.canvas.xview_moveto(0.0)
        self.root.protocol("WM_DELETE_WINDOW", self.close)
    def saveps(self):
        self.canvas.yview_moveto(0.0)
        self.canvas.xview_moveto(0.0)

        self.canvas.postscript(file="output.ps", colormode="color",height=self.yyy,width=self.yyy)

        

    def close(self):
        self.saveps()
        self.root.destroy()
print("\033c\033[47;30m\ngive me level file name \n")
a=input().strip()
f1=open(a,"r")
f=f1.read()
f1.close()

ff=f.split("\n")
xx=ff[0]
x=len(xx)
y=len(ff)
root = tk.Tk()

app = tApp(root,f,a,y,x)
root.mainloop()
