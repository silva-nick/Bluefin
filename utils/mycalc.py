class Token:
    def __init__(self, type, val) -> None:
        self.type = type
        self.val = val
        pass

class Lexer:
    def __init__(self, expr) -> None:
        self.expr = expr
        self.pos = 0
        pass
    def removeWhiteSpace(self):
        while self.pos < len(self.expr) and self.expr[self.pos] == ' ':
            self.pos += 1
        pass
    def integer(self):
        val = ""
        while self.pos < len(self.expr) and self.expr[self.pos].isdigit():
            val += self.expr[self.pos]
            self.pos += 1
        return int(val)
    def getNextToken(self):
        if (self.pos >= len(self.expr)):
            return Token("END", "")

        nextChar = self.expr[self.pos]
        if nextChar == ' ':
            self.removeWhiteSpace()
            return self.getNextToken()
        elif nextChar.isdigit():
            return Token("INT", self.integer())
        elif nextChar == "+":
            return Token("PLUS", "+")
        elif nextChar == "-":
            return Token("MIN", "-")        
        elif nextChar == "*":
            return Token("MULT", "*")
        elif nextChar == "/":
            return Token("DIV", "/")    
        elif nextChar == "(":
            return Token("PSTR", "(")
        elif nextChar == ")":
            return Token("PEND", ")")
        else:
            return Token("END", "")

class Interpreter:
    def __init__(self, lexer):
        self.lexer = lexer
        self.currToken = self.lexer.getNextToken()
    # consume 
    def eat(self, type):
        if self.currToken.type == type:
            self.currToken = self.lexer.getNextToken()
        else:
            print("FAIL!!!!!!!!!!!")
    # factor : Integer | OpenParen expr EndParen
    def factor(self):
        if (self.currToken.type == "PSTR"):
            self.eat("PSTR")
            val = self.expr()
            self.eat("PEND")
            return val
        else: 
            val = self.currToken.val
            self.eat("INT")
            return val
    # term : Integer((*|/)Integer)*
    def term(self):
        res = self.factor()
        while self.currToken.type in ("MULT", "DIV"):
            if self.currToken.type == "MULT":
                self.eat("MULT")
                res *= self.factor()
            else:
                self.eat("DIV")
                res /= self.factor()
        return res  
    # expr : term((+|-)term)*
    def expr(self):
        res = self.factor()
        op = self.currToken.type
        while op in ("PLUS", "MIN"):
            if op == "PLUS":
                self.eat("PLUS")
                res += self.term()
            else:
                self.eat("MIN")
                res -= self.term()
        return res  

def main():
    while True:
        try:
            text = input('calc> ')
        except EOFError:
            break
        if not text:
            continue
        lexer = Lexer(text)
        interpreter = Interpreter(lexer)
        result = interpreter.expr()
        print(result)


if __name__ == '__main__':
    main()