import sys, chardet

style = """
<style>
     .body {
         align: center;
         background-color: #bfbfbf;
     }

      .commit {
          background-color: #efefef;
          width: 50%;
          align: center;
          align-self: center;
          align-content: center;
          align-items: center;
          border-color: black;
          border-style: groove;
          border-radius: 25px;
          padding: 20px;
     }

     .em {
       cursor: pointer;
     }

     .spacer {
         height: 5%;
     }

</style>
"""

def indexInStr(index, string):
    out = 0
    for i in range(len(string)):
        if string[i] == index:
            break
        out += 1
    return out

def styleContent(date, commitkey, author, description, filesModified, filesAdded, filesDeleted):
    out = "<div class='commit'>"
    out += "<p><b>DATE:</b> " + date + "</p>\n"
    out += "<p><b>AUTHOR:</b> " + author + "</p>\n"
    out += "<p><b>KEY:</b> " + commitkey + "</p>\n"
    out += "<button style='color: #0000EE; shape: round;' onclick='window.open(\"https://github.com/iuyte/VEX-709s/tree/" + commitkey + "\")'>" + "Browse at this point in repo" + "</button>\n"
    out += "<p><b>DESCRIPTION:</b><br>" + description + "</p>\n"
    if len(filesAdded) > 0:
        out += "<b>Files added:</b>\n<ul>\n"
        for i in range(len(filesAdded)):
            out += "<li>" + filesAdded[i] + "</li>\n"
        out += "</ul>"
    if len(filesModified) > 0:
        out += "<b>Files modified:</b>\n<ul>\n"
        for i in range(len(filesModified)):
            out += "<li>" + filesModified[i] + "</li>\n"
        out += "</ul>"
    if len(filesDeleted) > 0:
        out += "<b>Files deleted:</b>\n<ul>\n"
        for i in range(len(filesDeleted)):
            out += "<li>" + filesDeleted[i] + "</li>\n"
        out += "</ul>"
    out += "<br></div>\n"
    return out

def removeSpaces(content):
    content = content.split(" ")
    out = ""
    for i in range(len(content)):
        out += content[i]
    return out

def parseContent(content):
    if content == "":
        assert False, "File is empty or does not exist"
    out = "<html>\n<head>\n<title>Git Commit history</title>\n</head>\n<body style='align: center; background-color: #bfbfbf;'>" + style
    count = 0
    commits = content.split("commit ")
    out += "<div class='commit'><h1>" + str(len(commits)) + " Commits</h1></div>\n<br>\n"
    commit = 0
    for commit in range(1, len(commits)):
        aline = 0
        lines = commits[commit].split("\n")
        try:
            lines[aline]
        except:
            continue
        line = ""
        lline = ""
        date = ""
        commitkey = ""
        author = ""
        description = ""
        filesModified = []
        filesAdded = []
        filesDeleted = []
        try:
            commitkey = lines[aline]
            aline += 1
            author = lines[aline].split("Author:")[1]
            aline += 1
            date = lines[aline].split("Date: ")[1]
            aline += 2
            description = lines[aline][4:]
            aline += 2
        except:
            continue
        filesModified = []
        filesAdded = []
        filesDeleted = []
        for aline in range(aline, len(lines)):
            if ". M" in lines[aline]:
                filesModified.append(lines[aline][39:])
            elif ". A" in lines[aline]:
                filesAdded.append(lines[aline][39:])
            elif ". D" in lines[aline]:
                filesDeleted.append(lines[aline][39:])
        out += styleContent(date, commitkey, author, description, filesModified, filesAdded, filesDeleted) + "<div class='spacer'></div>"
    out += "</body>\n</html>\n"
    return out
