from functools import cmp_to_key
from numpy.lib.shape_base import split
import pandas as pd


df = pd.read_csv('wordbook/freq_330k.csv')


def is_valid(phrase):
    words = phrase.split()
    return all(word.lower() in df['word'].values for word in words)


def frequency_of(phrase):
    words = phrase.split()
    frequencies = map(
        lambda word: df.loc[df['word'] == word.lower(), 'count'].item(), words)
    return sum(frequencies) / len(words)


def parse_category(line):
    category, phrases = line.split(':')
    phrases = list(set(phrases.split(',')))
    return category, list(filter(is_valid, phrases))


def write_categories(categories):
    with open('wordbook/categories.txt', 'w') as f:
        for category, phrases in categories.items():
            f.write(category)
            start = True
            for phrase in phrases:
                f.write(':' if start == True else ',')
                start = False
                f.write(phrase)
            f.write('\n')


def generate_categories():
    with open('wordbook/categories-origin.txt', 'r') as f:
        categories = {}
        for line in f:
            line = line.replace('\n', '')
            category, phrases = parse_category(line)
            categories[category] = set.union(categories[category], set(
                phrases)) if category in categories else set(phrases)

        for category, phrases in categories.items():
            categories[category] = sorted(
                phrases, key=frequency_of, reverse=True)

        write_categories(categories)


def split_levels(levels, line):
    category, phrases = line.split(':')
    phrases = phrases.split(',')
    index = 0
    length = 10
    while index < len(phrases):
        start = index if index == 0 else index - 5
        end = start + length if start + \
            length <= len(phrases) else len(phrases)
        if end - start < length:
            start = max(end - length, 0)
        levels.append((category, phrases[start:end]))
        index = end
        length += 10 if length < 50 else 0


def write_levels(levels):
    with open('wordbook/levels.txt', 'w') as f:
        for level in levels:
            f.write(level[0])
            start = True
            for phrase in level[1]:
                f.write(':' if start == True else ',')
                start = False
                f.write(phrase)
            f.write('\n')


def compare_level(l1, l2):
    if len(l1[1]) < len(l2[1]):
        return -1
    elif len(l1[1]) > len(l2[1]):
        return 1
    else:
        f1 = sum(map(lambda phrase: frequency_of(phrase), l1[1])) / len(l1[1])
        f2 = sum(map(lambda phrase: frequency_of(phrase), l2[1])) / len(l2[1])
        if f1 > f2:
            return -1
        elif f1 < f2:
            return 1
        else:
            return 0


def generate_levels():
    with open('wordbook/categories.txt', 'r') as f:
        levels = []
        for line in f:
            line = line.replace('\n', '')
            split_levels(levels, line)

        levels = sorted(levels, key=cmp_to_key(compare_level))
        write_levels(levels)


# generate_categories()
generate_levels()
