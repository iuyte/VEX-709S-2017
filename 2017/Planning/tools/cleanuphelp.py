import sys, chardet

style = """<link href="https://fonts.googleapis.com/css?family=Open+Sans" rel="stylesheet">
<style>
    .body {
        align: center;
        background-color: #bfbfdf;
        width: 100%;
        margin: 0px;
        font-family: 'Open Sans', sans-serif;
    }

    .link {
       color: black;
       text-decoration: none;
    }

    .link:visited {
       color: black;
       text-decoration: none;
    }

    .link:hover {
       color: #0000EE;
       text-decoration: underline;
    }

    .commit {
        background-color: #efefef;
        width: 50%;
        align: center;
        align-self: center;
        align-content: center;
        align-items: center;
        border-color: #454585;
        border-style: groove inset;
        border-radius: 25px;
        padding: 20px;
        margin-left: 25%;
        margin-right: 25%;
        box-shadow: 10px 10px 5px #777777;
    }

    .em {
        cursor: pointer;
    }

    .spacer {
        height: 5%;
    }

    .bcon {
       width: 8%;
    }

    .browse {
       background-color: #454585;
       color: white;
       border: 2px solid #454585;
       border-radius: 4px;
       position: absolute;
       right: 25%;
       display: none;
    }

    .browse:hover {
       background-color: #bfbfdf;
       border-color: #9d9dbf;
       color: black;
    }

    .bcon:hover .browse {
       display: inline;
    }

    .cwa {
       width: 29px;
       height: 29px;
       visibility: visible;
    }

    .hid {
       visibility: hidden;
    }

    .hid:hover {
       visibility: visible;
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
    out = "<div class='commit' id='" + commitkey + "'>"
    out += "<p><b>Commit:</b> <a class='link' href='https://iuyte.github.io/VEX-709s/2017/Planning/tools/git.html#" + commitkey + "'>" + commitkey + "</a>"
    out += "&nbsp&nbsp<span class='cwa'><img class='hid' src='link.png'></span></p>"
    #out += "<div class='bcon'><button class='browse' onclick='window.open(\"https://github.com/iuyte/VEX-709s/tree/" + commitkey + "\")'>" + "Browse at this point in repo" + "</button></div></p>\n"
    out += "<p><b>Date:</b> " + date + "</p>\n"
    out += "<p><b>Author:</b> " + author + "</p>\n"
    out += "<p><b>Description:</b><br>" + description + "</p>\n"
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
    script = ""#script1 + script2.format(commitkey) + script3
    out += script + "<br></div>\n"
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
    out = "<html>\n<head>\n" + style + "<title>Git Commit history</title>\n</head>\n<body style='background-color: #bfbfdf;' class='body'>"
    count = 0
    commits = content.split("commit ")
    out += "<div class='commit' id='top'><h1>" + str(len(commits)) + " Commits</h1></div>\n<br>\n"
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
