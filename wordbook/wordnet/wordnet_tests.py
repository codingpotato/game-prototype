import unittest
from wordnet import Wordnet


class TestWordnet(unittest.TestCase):
    wn = Wordnet()

    def test_constructor(self):
        self.assertEqual(len(self.wn.all_words), 9884)

    def test_valid_chars(self):
        self.assertTrue(self.wn.valid_chars('book-_'))
        self.assertFalse(self.wn.valid_chars("book's"))
        self.assertFalse(self.wn.valid_chars("book1"))

    def test_valid_words(self):
        self.assertTrue(self.wn.valid_words(['am', 'are', 'is']))
        self.assertFalse(self.wn.valid_words(['abcd', 'am', 'are', 'is']))

    def test_is_valid(self):
        self.assertTrue(self.wn.is_valid('book-word'))
        self.assertFalse(self.wn.is_valid('book-word-book-word'))

    def test_similar_word(self):
        swords = self.wn.similar_words('word')
        self.assertNotEqual(len(swords), 0)

    def test_sort_by_similarity(self):
        sorted = self.wn.sort_by_similarity(
            'health', ['them', 'number', 'welfare'])
        self.assertEqual(sorted, ['welfare', 'number', 'them'])


if __name__ == '__main__':
    unittest.main()
