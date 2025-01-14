from functools import cmp_to_key
from numpy.lib.shape_base import split
import pandas as pd


def read_frequency():
    with open('Level/freq_330k.csv') as f:
        freq = {}
        for line in f:
            line = line.replace('\n', '')
            word, count = line.split(',')
            freq[word] = int(count)
        return freq


freq = read_frequency()


def is_valid(phrase):
    words = phrase.split()
    return all(word.lower() in freq for word in words)


def frequency_of(phrase):
    words = phrase.split()
    frequencies = list(map(
        lambda word: freq[word.lower()], words))
    return min(frequencies) - len(frequencies) + 1


def split_levels(levels, line):
    category, phrases = line.split(':')
    phrases = phrases.split(',')
    start = 0
    length = 10
    while start < len(phrases):
        end = start + length if start + \
            length <= len(phrases) - length / 2 else len(phrases)
        words = phrases[start:end]
        frequency = sum(map(lambda phrase: frequency_of(phrase), words)) / len(words)
        print(category, frequency, words)
        levels.append((category, words, frequency))
        start = end
        length += 5 if length < 30 else 0


def write_levels(levels):
    with open('Level/level.data', 'w') as f:
        for level in levels:
            f.write(level[0])
            start = True
            for phrase in level[1]:
                f.write(':' if start == True else ',')
                start = False
                f.write(phrase)
            f.write('\n')


def generate_levels():
    with open('Level/categories.txt', 'r') as f:
        levels = []
        for line in f:
            line = line.replace('\n', '')
            split_levels(levels, line)

        levels = sorted(levels, key=lambda level: level[2], reverse=True)
        write_levels(levels)


generate_levels()
