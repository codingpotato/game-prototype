import re
from nltk.corpus import wordnet as wn


valid_words = []


def is_valid(word):
    global valid_words
    if len(valid_words) == 0:
        f = open('words.txt', 'r')
        data = f.read()
        valid_words = data.split()
    words = word.split('-_')
    return len(word) <= 18 and all(c.isalpha() or c == '-' or c == '_' for c in word) and word in valid_words and all(word in valid_words for word in words)


def related_word(word):
    used_words = set()
    used_words.add(word)
    synonyms = set()
    antonyms = set()
    hypernyms = set()
    hyponyms = set()
    synsets = wn.synsets(word)
    for synset in synsets:
        for lemma in synset.lemmas():
            if not lemma.name() in used_words and is_valid(lemma.name()):
                used_words.add(lemma.name())
                synonyms.add(lemma.name())
            for antonym in lemma.antonyms():
                if not antonym.name() in used_words and is_valid(lemma.name()):
                    used_words.add(antonym.name())
                    antonyms.add(antonym.name())
        for hypernym in synset.hypernyms():
            for name in hypernym.lemma_names():
                if not name in used_words and is_valid(lemma.name()):
                    used_words.add(name)
                    hypernyms.add(name)
        for hyponym in synset.hyponyms():
            for name in hyponym.lemma_names():
                if not name in used_words and is_valid(lemma.name()):
                    used_words.add(name)
                    hyponyms.add(name)
    return (synonyms, antonyms, hypernyms, hyponyms)


def write_strings(f, strings):
    index = 0
    for str in strings:
        f.write(str)
        if index < len(strings) - 1:
            f.write(',')
        index += 1


def parse_word():
    f = open('wordnet.txt', 'w')
    words = set()
    is_valid('good')
    for word in valid_words:
        if not word in words:
            words.add(word)
            (synonyms, antonyms, hypernyms, hyponyms) = related_word(word)
            f.write(word)
            f.write('|')
            write_strings(f, synonyms)
            f.write('|')
            write_strings(f, antonyms)
            f.write('|')
            write_strings(f, hypernyms)
            f.write('|')
            write_strings(f, hyponyms)
            f.write('\n')
    f.close()


parse_word()
