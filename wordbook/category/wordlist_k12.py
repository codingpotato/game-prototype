import re
import ssl
from retrying import retry
import urllib.request

wordlist_re = re.compile(
    r'<a href="/wordlist/(\w+.shtml)" target="_top">\w+</a><BR>')
wordlist_name_re = re.compile(
    r'<h1 class=body-title__title>(.+?)</h1>')
words_re = re.compile(
    r'<div class=wordlist-item>([\w -\\\']+?)</div>')


def extract_wordlist(page):
    name = wordlist_name_re.search(page)
    return name.group(1).strip(), words_re.findall(page)


@retry
def get_wordlist(url_name):
    try:
        context = ssl.create_default_context()
        context.check_hostname = False
        context.verify_mode = ssl.CERT_NONE
        response = urllib.request.urlopen(
            'https://www.enchantedlearning.com/wordlist/' + url_name, context=context)
        return extract_wordlist(str(response.read()))
    except Exception as error:
        print(error)


def write_to_file(f, name, words):
    f.write(name)
    first = True
    for word in words:
        f.write(':' if first == True else ',')
        first = False
        f.write(word)
    f.write('\n')
    f.flush()


def get_wordlists_k12():
    context = ssl.create_default_context()
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE
    with urllib.request.urlopen('https://www.enchantedlearning.com/wordlist/', context=context) as response:
        with open('wordbook/category/categories-k12.txt', 'w') as f:
            for url_name in wordlist_re.findall(str(response.read())):
                name, words = get_wordlist(url_name)
                print(name)
                write_to_file(f, name, words)


get_wordlists_k12()
