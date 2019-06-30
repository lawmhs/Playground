import nltk
from nltk.corpus import stopwords

# to be run by node 1
def tokenize(fileName):
    s = ""
    with open(fileName, 'r') as f:
        for line in f:
            s += " " + line
    ret = nltk.word_tokenize(s)
    return ret

# to be run by node 2
def filterCorpus(words):
    stop_words = set(stopwords.words('english'))
    words_without_punctuation = [w for w in words if w.isalpha()]
    # at the start of the below code, lower() should not error out since we'll get rid of punctuation first
    result = [w for w in words_without_punctuation if w.lower() not in stop_words]
    return result

    

    
        
