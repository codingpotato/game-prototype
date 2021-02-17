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


def parse_category(line):
    category, phrases = line.split(':')
    phrases = list(set(phrases.split(',')))
    return category, list(filter(is_valid, phrases))


def write_categories(categories):
    with open('Level/categories.txt', 'w') as f:
        for category, phrases in categories.items():
            f.write(category)
            start = True
            for phrase in phrases:
                f.write(':' if start == True else ',')
                start = False
                f.write(phrase)
            f.write('\n')


def generate_categories():
    with open('Level/categories-original.txt', 'r') as f:
        categories = {}
        for line in f:
            line = line.replace('\n', '')
            category, phrases = parse_category(line)
            categories[category] = set.union(categories[category], set(
                phrases)) if category in categories else set(phrases)

        for category, phrases in categories.items():
            print('sort: ', category)
            categories[category] = sorted(phrases, key=frequency_of, reverse=True)
            print(categories[category])
        write_categories(categories)


generate_categories()
