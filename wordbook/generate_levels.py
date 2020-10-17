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


def generate_levels():
    with open('wordbook/categories-origin.txt', 'r') as f:
        categories = {}
        for line in f:
            line = line.replace('\n', '')
            category, phrases = parse_category(line)
            print(category, ':', phrases)
            categories[category] = set.union(categories[category], set(
                phrases)) if category in categories else set(phrases)

        max_count = 0
        for category, phrases in categories.items():
            phrases = sorted(phrases, key=frequency_of, reverse=True)
            max_count = max(max_count, len(phrases))
            print(category, ':', len(phrases), '->', phrases)
        print(len(categories))
        print(max_count)


generate_levels()
