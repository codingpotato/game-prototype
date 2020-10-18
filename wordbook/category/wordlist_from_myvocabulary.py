import re
from retrying import retry
import urllib.request

wordlist_re = re.compile(
    r'<a href="word-list/([\w-]+)/" style="text-decoration')
wordlist_name_re = re.compile(
    r'<h2>(.+?) Vocabulary Word List \(\d+\)</h2>')
words_re = re.compile(
    r'<td valign=\\\'top\\\' align=\\\'left\\\'>(.*?)</td>')


def extract_wordlist(page):
    name = wordlist_name_re.search(page)
    words = []
    for string in words_re.findall(page):
        for word in string.split(','):
            word = word.strip().lower()
            if len(word) > 0:
                words.append(word)
    print(name.group(1), words)
    return name.group(1), words


@retry
def get_wordlist(url_name):
    response = urllib.request.urlopen(
        'https://myvocabulary.com/word-list/' + url_name + '/')
    return extract_wordlist(str(response.read()))


def write_to_file(f, name, words):
    f.write(name)
    first = True
    for word in words:
        f.write(':' if first == True else ',')
        first = False
        f.write(word)
    f.write('\n')


def get_wordlists_from_my_vocabulary():
    with urllib.request.urlopen('https://myvocabulary.com/dir-wordlist-word_list_alphabetic') as response:
        with open('wordbook/category/categories-my-vocabulary.txt', 'w') as f:
            for url_name in wordlist_re.findall(str(response.read())):
                name, words = get_wordlist(url_name)
                write_to_file(f, name, words)


get_wordlists_from_my_vocabulary()
