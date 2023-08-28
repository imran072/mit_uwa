def main(filename):
    intro()
    fileData = readfile(filename)
    fileText = dataClean(fileData)
    wordDic = wordCount(fileText)
   # display(wordDic)
    

def intro():
    print("this is a demo shown in the class which is a script that counts words")
    print("The output will be printed")
    print()
    
def readfile(f):
    try:
        fread = open(f,"r")
        fileText = fread.read()
    except IOError:
        print("the program can't access the file")
        print("program terminated!!!")
        return ""
    fread.close()
    return fileText

def dataClean(txt):
    txt = txt.lower()
    sp_ch = "!@#$%^&*()-_+={ }[]|\/:;'<>,.?~`" + '"'
    for ch in sp_ch:
        txt = txt.replace(ch, " ")
    return txt

def wordCount(txt):
    words = txt.split()
    wfreq = {}
    for word in words:
        if word in wfreq:
            wfreq[word] += 1
        else:
            wfreq[word] = 1
    return wfreq
'''
def display(wdict):
    print("words: frequency")
    for k in wdict:
        print(k, wdict[k])
'''