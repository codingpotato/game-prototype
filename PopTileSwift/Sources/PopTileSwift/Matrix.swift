//
//  Matrix.swift
//  PopTile
//
//  Created by Wang Yongwu on 2019/11/11.
//  Copyright © 2019 Wang Yongwu. All rights reserved.
//

import Foundation

struct Matrix<Element> : Sequence {
    struct RowView : Sequence, IteratorProtocol {
        let matrix: Matrix<Element>
        let row: Int
        var column = 0

        init(matrix: Matrix<Element>, row: Int) {
            self.matrix = matrix
            self.row = row
        }
        
        func makeIterator() -> RowView {
            return self
        }
        
        mutating func next() -> Element? {
            if column == matrix.columns {
                return nil
            }
            let value = matrix[row, column]
            column += 1
            return value
        }
    }
    
    struct ColumnView : Sequence, IteratorProtocol {
        let matrix: Matrix<Element>
        let column: Int
        var row = 0

        init(matrix: Matrix<Element>, column: Int) {
            self.matrix = matrix
            self.column = column
        }
        
        func makeIterator() -> ColumnView {
            return self
        }
        
        mutating func next() -> Element? {
            if row == matrix.rows {
                return nil
            }
            let value = matrix[row, column]
            row += 1
            return value
        }
    }
    
    let rows: Int
    let columns: Int
    
    private var elements: [Element];

    init(rows: Int, columns: Int, repeating: Element) {
        self.rows = rows
        self.columns = columns
        self.elements = [Element](repeating: repeating, count: rows * columns)
    }
    
    init(rows: Int, columns: Int, elements: [Element]) {
        self.rows = rows
        self.columns = columns
        self.elements = elements
    }
    
    subscript(row: Int, column: Int) -> Element {
        get {
            return elements[indexOfRow(row, column: column)]
        }
        set(newValue) {
            elements[indexOfRow(row, column: column)] = newValue
        }
    }
    
    func makeIterator() -> Array<Element>.Iterator {
        return elements.makeIterator()
    }
    
    func forEachRow(_ body: (Int, RowView) -> Void) {
        for row in 0..<rows {
            body(row, RowView(matrix: self, row: row))
        }
    }
    
    func forEachColumn(_ body: (Int, ColumnView) -> Void) {
        for column in 0..<columns {
            body(column, ColumnView(matrix: self, column: column))
        }
    }
    
    func viewOfRow(_ row: Int) -> RowView {
        return RowView(matrix: self, row: row)
    }
    
    func viewOfColumn(_ column: Int) -> ColumnView {
        return ColumnView(matrix: self, column: column)
    }
    
    private func indexOfRow(_ row: Int, column: Int) -> Array<Element>.Index {
        return row * columns + column;
    }
}
