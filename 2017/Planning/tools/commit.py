import jsonpickle
class Commit(object):
    date = ""
    commitkey = ""
    author = ""
    description = ""
    filesModified = []
    filesAdded = []
    filesDeleted = []
    def __init__(self, date, commitkey, author, description, filesModified, filesAdded, filesDeleted):
        self.date = date
        self.commitkey = commitkey
        self.author = author
        self.description = description
        self.filesModified = filesModified
        self.filesAdded = filesAdded
        self.filesDeleted = filesDeleted

def printCommit(commit):
    frozen = jsonpickle.encode(commit)
    print(frozen)

def saveList(listOfCommits):
    f = open("log.json", mode="w")
    toWrite = jsonpickle.encode(listOfCommits)
    f.write(toWrite)
    f.close()

class Commits(object):
    def __init__(self, *commits):
        i = 0
        for commit in commits:
            print(commit)
