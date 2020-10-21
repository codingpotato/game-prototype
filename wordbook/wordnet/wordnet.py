import operator
import re
import nltk
from nltk.corpus import wordnet as wn


class Wordnet:
    def __init__(self):
        with open('words.txt', 'r') as f:
            self.all_words = list(
                filter(lambda w: len(w) >= 4, f.read().split()))
        self.index = nltk.text.ContextIndex(
            [w.lower() for w in nltk.corpus.brown.words()])

    def valid_chars(self, word):
        return all(c.isalpha() or c == '-' or c == '_' for c in word)

    def valid_words(self, words):
        return all(w in self.all_words for w in words)

    def is_valid(self, original_word, word):
        subwords = re.split('[-_]', word)
        return len(word) <= 18 and word != original_word and self.valid_chars(word) and self.valid_words(subwords)

    def try_add(self, original_word, word, result):
        if self.is_valid(original_word, word):
            result.add(word)

    def similar_words(self, word):
        result = set()
        synsets = wn.synsets(word)
        for synset in synsets:
            for lemma in synset.lemmas():
                self.try_add(word, lemma.name(), result)
                for antonym in lemma.antonyms():
                    self.try_add(word, antonym.name(), result)
            for hypernym in synset.hypernyms():
                for name in hypernym.lemma_names():
                    self.try_add(word, name, result)
            for hyponym in synset.hyponyms():
                for name in hyponym.lemma_names():
                    self.try_add(word, name, result)
        return result

    def add_similar(self, word, swords):
        words = set(filter(lambda w: self.is_valid(word, w),
                           self.index.similar_words(word)))
        swords |= words

    def similarity(self, word1, word2):
        best = 0
        for s1 in wn.synsets(word1):
            for s2 in wn.synsets(word2):
                s = wn.wup_similarity(s1, s2)
                if s:
                    best = max(s, best)
        return best

    def sort_by_similarity(self, word, swords):
        similarities = map(lambda w: self.similarity(word, w), swords)
        items = list(zip(similarities, swords))
        items = sorted(items, key=operator.itemgetter(0), reverse=True)
        return list(map(operator.itemgetter(1), items))

    def output(self, word, swords):
        with open('wordnet.txt', 'w') as f:
            f.write(word)
            f.write(':')
            for w in swords:
                f.write(w)
                f.write(',')
            f.write('\n')

    def generate_wordnet(self):
        for word in self.all_words:
            swords = self.similar_words(word)
            if (len(swords) < 20):
                self.add_similar(word, swords)
            swords = self.sort_by_similarity(word, swords)


if __name__ == '__main__':
    Wordnet().generate_wordnet()
