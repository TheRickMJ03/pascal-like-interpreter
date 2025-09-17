
#include <stack>

#include "parserInterp.h"
#include"lex.h"



map<string, bool> defVar;
map<string, Token> SymTable;
vector<pair<string, bool>> varList;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects
stack<Value>power;

string initvar;
int flag = 0;
int value=1;
int flagi =0;
LexItem TYPEK;
int nlevel;

bool falseflag = true;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line);


// 1. Prog ::= PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if (t != PROGRAM) {
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != IDENT) {
        ParseError(line, "Missing PROgram Name");

        return false;
    }


    // Parse declarations
    while (Decl(in, line)) {	}
    
    
      
    
   
     

    // Parse statements
    while (Stmt(in, line)) {	}
    

// Print the keys and values of SymTable
cout << "Variables in SymTable:" << endl;
for (const auto& entry : SymTable) {
    const string& key = entry.first;         // Get the key (string)
    const Token& value = entry.second;       // Get the value (Token)
    cout << "Variable: " << key << ", Value: " << value << endl;
}


    // Check for END PROGRAM IDENT
    t = Parser::GetNextToken(in, line);
    if (t != END) {
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != PROGRAM) {
        ParseError(line, "PROGRAM token was not found");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != IDENT) {
        ParseError(line, "IDENT was not found");
        return false;
    }

   



	cout<<"(DONE)"<<endl;
    return true;
}

//End of Prog

// 2. Decl ::= Type :: VarList
bool Decl(istream& in, int& Line){
	if(!Type(in,Line)){	

		return false;
		
	}

	LexItem t = Parser::GetNextToken(in, Line);
	if(t != DCOLON){
		ParseError(Line, "MISSING DCOLON");
		return false;
	}

     LexItem  idtok;

	if(!VarList(in, Line, idtok, value)){ // Pass strlen as an argument
    ParseError(Line, "MISSING VarList()");
    return false;
}


	return true;
}
//End of Decl


// 3. Type ::= INTEGER | REAL | CHARARACTER [(LEN = ICONST)]
bool Type(istream& in, int& Line) {

    LexItem t = Parser::GetNextToken(in, Line);
	
    if (t == INTEGER || t == REAL) {
        TYPEK = t;
        return true;
    } else if (t == CHARACTER) {
        TYPEK = t;
        t = Parser::GetNextToken(in, Line);
        if (t == LPAREN) {
            t = Parser::GetNextToken(in, Line);

            if (t != LEN) {
                ParseError(Line, "MISSING LEN");
                return false;
            }

            t = Parser::GetNextToken(in, Line);
            if (t != ASSOP) {
                ParseError(Line, "MISSING ASSOP");
                return false;
            }

            t = Parser::GetNextToken(in, Line);
            value = std::stoi(t.GetLexeme());

            if (t != ICONST) {
                ParseError(Line, "MISSING ICONST");
                return false;
            }

            t = Parser::GetNextToken(in, Line);
            if (t != RPAREN) {
                ParseError(Line, "MISSING RPAREN");
                return false;
            }
            
            flag=1;
            
            return true; // Length specification present and parsed
        } else {
            Parser::PushBackToken(t);


            return true; // Character type without length specification
        }
    }
    Parser::PushBackToken(t);

    return false;
}
//End of Type

// 4. VarList ::= Var [= Expr] {, Var [= Expr]}
bool VarList(istream& in, int& Line,LexItem & idtok,int strlen ) {

    
idtok = TYPEK;
    if (!Var(in, Line, idtok)) {
        ParseError(Line, "MISSING Var()");
        return false;
    }

  

    LexItem t = Parser::GetNextToken(in, Line);

    // Check if there is an optional expression
    if (t == ASSOP) {



        Value retVal;
        if (!Expr(in, Line, retVal)) {
            ParseError(Line, "MISSING Expr()");
            return false;
        }





    if(TYPEK.GetToken() == 4){

          retVal = double (retVal.GetInt());

        TempsResults[initvar] = retVal;

    }



   
    if (strlen == 1) {
        if (retVal.IsString()) {
            char tem = retVal.GetString()[0];
            TempsResults[initvar] = string(1, tem); // Accessing the first character of the string
        } else {
            TempsResults[initvar] = retVal;
        }
    } else if (retVal.IsString()) {
        string tempp = retVal.GetString();

        // Accessing the character at index 'strlen - 1'
        // Assuming 'strlen' is the correct index
        if (strlen <= tempp.size()) {
            char temlen = retVal.GetString()[strlen];
            TempsResults[initvar] = temlen;
        } else {

            int lenofstring = strlen-tempp.size();
            tempp += std::string(lenofstring, ' ');

            // Handle the case when 'strlen' is out of bounds
            TempsResults[initvar] = tempp;
        }
    }


        // Mark the variable as initialized since it has been sed a value
        varList.back().second = true; // Set the initialization status of the last variable in the vector to true

        t = Parser::GetNextToken(in, Line); // Fetch next token
    }

    // Process comma-separated list
    while (t == COMMA) {

        if (!Var(in, Line, idtok)) {
            ParseError(Line, "MISSING Var()");
            return false;
        }

        t = Parser::GetNextToken(in, Line); // Fetch next token

        // Check for optional expression after comma
        if (t == ASSOP) {
              Value retVal;
                if (!Expr(in, Line, retVal)) {
                    ParseError(Line, "MISSING Expr()");
                    return false;
                 }

                    if(TYPEK.GetToken() == 4){

                        retVal = double (retVal.GetInt());

                        TempsResults[initvar] = retVal;

                    }


                    if (strlen == 1) {
                        if (retVal.IsString()) {
                            char tem = retVal.GetString()[0];
                            TempsResults[initvar] = string(1, tem); // Accessing the first character of the string
                        } else {
                            TempsResults[initvar] = retVal;
                        }
                    } else if (retVal.IsString()) {
                        string tempp = retVal.GetString();

                        // Accessing the character at index 'strlen - 1'
                        // Assuming 'strlen' is the correct index
                                      if (value <= tempp.size()) {
                            string temlen = retVal.GetString().substr(0, value);
                            TempsResults[initvar] = temlen;
                        } else {

                            int lenofstring = strlen-tempp.size();
                            tempp += std::string(lenofstring, ' ');

                            // Handle the case when 'strlen' is out of bounds
                            TempsResults[initvar] = tempp;
                        }
                    }


                            
                        // Mark the variable as initialized since it has been assigned a value
                            varList.back().second = true; // Set the initialization status of the last variable in the vector to true
                            t = Parser::GetNextToken(in, Line); // Fetch next token
        }else{

            Value retVal;


            if (strlen == 1) {
                        if (retVal.IsString()) {
                            char tem = retVal.GetString()[0];
                            TempsResults[initvar] = string(1, tem); // Accessing the first character of the string
                        } else {
                            TempsResults[initvar] = retVal;
                        }
                    } else  {

                        retVal.SetType(VSTRING);

                        string tempp = retVal.GetString();

                        // Accessing the character at index 'strlen - 1'
                        // Assuming 'strlen' is the correct index
                        if (value <= tempp.size()) {
    string temlen = retVal.GetString().substr(0, value);
    TempsResults[initvar] = temlen;
} else {

            int lenofstring = strlen-tempp.size();
            tempp += std::string(lenofstring, ' ');

            // Handle the case when 'strlen' is out of bounds
            TempsResults[initvar] = tempp;
        }
                    }
          

        }
    }

    Parser::PushBackToken(t);
    return true;
}







//End of VarList


// 5. Stmt ::= AssigStmt | BlockIfStmt | PrintStmt | SimpleIfStmt
bool Stmt(istream& in, int& Line) {
    LexItem t = Parser::GetNextToken(in, Line);

    // Check if GetNextToken returned an ERR token
    if (t == ERR) {
        ParseError(Line, "ERROR in tokenization");
        return false;
    }

    if (t == END) {
        Parser::PushBackToken(t);
        return false;
    }

    if (t == PRINT) {
        // Attempt to parse as PrintStmt
        if (!PrintStmt(in, Line)) {
            return false;
        }
    } else if (t == IF) {
        // Attempt to parse as an IF statement
        LexItem next = Parser::GetNextToken(in, Line);
        if (next == LPAREN) {
            // Parse RelExpr
            Value retVal;
            if (!RelExpr(in, Line, retVal)) {
                return false;
            }

            next = Parser::GetNextToken(in,Line);
            if( next != RPAREN){
                ParseError(Line, "NO RPAREn");
		        return false;
            }


            // Check if the token after RelExpr is THEN
            LexItem thenToken = Parser::GetNextToken(in, Line);
            if (thenToken == THEN) {
                // Attempt to parse as a BlockIfStmt

                falseflag = retVal.GetBool();

                  if (!BlockIfStmt(in, Line)) {
                    return false;
                }

            } else {
                // Push back the tokens for further analysis
                Parser::PushBackToken(thenToken);
                // Attempt to parse as a SimpleIfStmt
                if (!SimpleIfStmt(in, Line)) {
                    return false;
                }
            }
        } else {
            // Unexpected token after IF
            ParseError(Line, "Expected '(' after IF");
            return false;
        }
    } else if (t == IDENT) {
        Parser::PushBackToken(t);

        // Attempt to parse as AssignStmt
        if (!AssignStmt(in, Line)) {
            ParseError(Line, "ERROR IN ASSIGNSTMT");
            return false;
        }
    } else {
        Parser::PushBackToken(t);
        return false;
    }
    
    // If the parsing attempt succeeds, return true
    return true;
}



//End of Stmt



//6. PrintStmt:= PRINT *, ExpreList 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>;

	
	t = Parser::GetNextToken(in, line);
	
 	if( t != DEF ) {
		
		ParseError(line, "Print statement syntax error.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	
	if( t != COMMA ) {
		
		ParseError(line, "Missing Comma.");
		return false;
	}
	
	if( !ExprList(in,line) ) {
		ParseError(line, "Missing expression after Print Statement");
		return false;
	}
 

    while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	return true;
}//End of PrintStmt

// 7. BlockIfStmt ::= IF (RelExpr) THEN {Stmt} [ELSE {Stmt}] END IF
bool BlockIfStmt(istream& in, int& line) {
    nlevel++; // Increment nesting level for each new IF block

    // Parse statements within the IF block
    if (falseflag == true) {
        while (Stmt(in, line)) {
            // No need to print message here; statements are being parsed
        }

        // Check if there's an ELSE part
        LexItem t = Parser::GetNextToken(in, line);
        if (t == ELSE) {
            // Parse statements within the ELSE block
            while (Stmt(in, line)) {
                // No need to print message here; statements are being parsed
            }
            t = Parser::GetNextToken(in, line); // Fetch the token after ELSE block
        }

        // Check for the end of the IF block
        if (t != END) {
            ParseError(line, "Missing END token after IF block");
            return false;
        }
    } else {
        // Skip tokens until reaching ELSE or END
        LexItem t;
        do {
            t = Parser::GetNextToken(in, line);
        } while (t != ELSE && t != END);

        if (t == ELSE) {
            // Parse statements within the ELSE block
            while (Stmt(in, line)) {
                // No need to print message here; statements are being parsed
            }
            t = Parser::GetNextToken(in, line); // Fetch the token after ELSE block
        }

        // Check for the end of the IF block
        if (t != END) {
            ParseError(line, "Missing END token after IF block");
            return false;
        }
    }

    // Check for the IF token after the END token
    LexItem t = Parser::GetNextToken(in, line);
    if (t != IF) {
        ParseError(line, "Missing IF token after END token");
        return false;
    }

    // Print message indicating the end of the current IF block
    nlevel--; // Decrement nesting level after finishing the IF block
    return true;
}


//End of BlockIfStmt

// 8. SimpleIfStmt ::= IF (RelExpr) SimpleStmt
bool SimpleIfStmt(istream& in, int& line) {
    // Here we assume that the current token represents the IF keyword


    if (!SimpleStmt(in, line)) {
        ParseError(line, "Error in SimpleStmt");
        return false;
    }



    return true;
}


//End of SimpleIfStmt

// 9. SimpleStmt ::= AssigStmt | PrintStmt
bool SimpleStmt(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if (t == PRINT) {
        return PrintStmt(in, line);
    } else if (t == IDENT) {
        in.putback(t.GetLexeme()[0]); // Put back the identifier for further parsing
        return AssignStmt(in, line);
    } else {
        ParseError(line, "ERROR in parsing SimpleStmt()");
        return false;
    }
}



// 10. AssignStmt ::= Var = Expr
bool AssignStmt(istream& in, int& line){


	LexItem idtok;
    idtok = TYPEK;
 	if(!Var(in,line, idtok)) {
		
		ParseError(line, "MISSING Var() token");
		return false;
	}
    
    LexItem t = Parser::GetNextToken(in, line);
	if( t != ASSOP){
		ParseError(line, "MISSING ASSOPqq");
		return false;
	}



    Value retVal;

       
	if(!Expr(in,line,retVal)) {
		
		ParseError(line-1, "MISSING Expr token");
		return false;
	}

     cout<<"THIS IS INITVAR "<< initvar<<endl;
    cout<<"RETVAL AFTER EXPREEEEEEEE "<< retVal<<endl;
    cout<<"THIS IS IDTOK "<<idtok<<endl;

 
     if(retVal.IsErr()){
            
            ParseError(line-1, "Using uninitialized Variable");
            return false;

     }
  



    if(SymTable.find(initvar)->second==CHARACTER&& !retVal.IsString()){
        ParseError(5, "MIXED MODE");
            return false;
    }

    


    if (SymTable.find(initvar)->second ==REAL) {
       
        TempsResults[initvar] = retVal;
        return true;


       
    }

          


    
   



   
    if (value == 1) {
        if (retVal.IsString()) {
            char tem = retVal.GetString()[0];
            TempsResults[initvar] = string(1, tem); // Accessing the first character of the string
        } else {
            TempsResults[initvar] = retVal;
        }
    } else if (retVal.IsString()) {
        string tempp = retVal.GetString();

        // Accessing the character at index 'strlen - 1'
        // Assuming 'strlen' is the correct index
       if (value <= tempp.size()) {
    string temlen = retVal.GetString().substr(0, value);
    TempsResults[initvar] = temlen;
}else {

            int lenofstring = value-tempp.size();
            tempp += std::string(lenofstring, ' ');

            // Handle the case when 'strlen' is out of bounds
            TempsResults[initvar] = tempp;
        }
    }

	return true;

}
//End of AssignStmt

// 16. SFactor ::= [+ | -] Factor
bool SFactor(istream& in, int& line, Value & retVal) {
    int sign = 1; // Default sign is positive
    
    LexItem t = Parser::GetNextToken(in, line);
    // Check if it's a positive or negative sign
    if (t == PLUS) {
        // Positive sign does not change the default sign (1)
    } else if (t == MINUS) {
        // Negative sign changes the default sign to -1
        sign = -1;

    } else {
        // If it's neither a positive nor a negative sign, push back the token
        // and proceed to parse the factor with the default sign (1)
        Parser::PushBackToken(t);
    }

    // Parse the factor with the determined sign
    if (!Factor(in, line, sign, retVal)) {
        ParseError(line, "Error in Factor");
        return false;
    }

    return true;
}

// // //End of SFactor
// bool TermExpr(istream& in, int& line, Value & retVal){

// 	if (!SFactor(in, line, retVal)) {
//         ParseError(line, "Error in SFactor");
//         return false;
//     }

//     LexItem t = Parser::GetNextToken(in, line);

//     while (t == POW) { 
//         Value nextval;
//         if (!SFactor(in, line, nextval)) {
//             ParseError(line, "Error in SFactor");
//             return false;
//         }
//         cout<<"THIS IS THE TYPE OF RETVAL: "<<retVal.GetType()<<endl;
//         cout<<"THIS IS THE VALUE OF RETVAL: "<<retVal.GetReal()<<endl;

//         cout<<"THIS IS THE TYPE OF NEXTVAL: "<<nextval.GetType()<<endl;
//         cout<<"THIS IS THE VALUE  OF NEXTVAL: "<<nextval.GetReal()<<endl;

//         retVal = retVal.Power(nextval);
//         cout<<"retval after operation  "<<retVal.GetReal()<<endl;

//         t = Parser::GetNextToken(in, line);
//         cout<<"THIS IS T  "<<t<<endl;
//     }

  
    

//     Parser::PushBackToken(t); // Push back the token which is not part of the current expression

//     return true;
// }


bool TermExpr(istream& in, int& line, Value & retVal){

	if (!SFactor(in, line, retVal)) {
        ParseError(line, "Error in SFactor");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);
    if(t==POW){
    while (t == POW) { 
        Value nextval;
        if (!SFactor(in, line, nextval)) {
            ParseError(line, "Error in SFactor");
            return false;
        }

        power.push(nextval);



        t = Parser::GetNextToken(in, line);
        // cout<<"THIS IS T  "<<t<<endl;
    }

        Value TORIGHT=1.00; // Store the base as the initial value of retVal
        while (!power.empty()) {
            Value nextFactor = power.top();
            power.pop();
            // Perform the power operation with the current factor as the exponent and the base as the base
            TORIGHT = nextFactor.Power(TORIGHT);// Update the base for the next iteration

        }

        retVal = retVal.Power(TORIGHT);



    }
    

    Parser::PushBackToken(t); // Push back the token which is not part of the current expression

    return true;
}


// 14. MultExpr ::= TermExpr { ( * | / ) TermExpr }
bool MultExpr(istream& in, int& line, Value & retVal){
	 if (!TermExpr(in, line, retVal)) {
        ParseError(line, "Error in TermExpr");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    while (t == MULT || t == DIV) { 
        Value nextval;

        if (!TermExpr(in, line, nextval)) {
            ParseError(line, "Error in TermExpr");
            return false;
        }


        if(t == MULT){
            cout<<"THIS IS RETVAL "<<retVal<<endl;
            cout<<"THIS IS NEXTVAL "<<nextval<<endl;
            retVal = retVal * nextval;

            cout<<"AFTER THE MULT "<< retVal<<endl;
        }else if (t == DIV){
            if(nextval.GetReal()==0){

                return false;
            }
            

             retVal = retVal / nextval;
        }


        t = Parser::GetNextToken(in, line);
    }

    Parser::PushBackToken(t); // Push back the token which is not part of the current expression
    return true;
}
//End of MultExpr


//11. ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
    Value retVal;

    if(!Expr(in,line,retVal)){
		ParseError(line, "Missing Expression");
        return false;

    }

    ValQue->push(retVal);

    LexItem tok = Parser::GetNextToken(in, line);
    while(tok ==COMMA){
        if(!Expr(in,line,retVal)){
            ParseError(line, "Missing Expression");
            return false;
        }
        
    ValQue->push(retVal);

    tok = Parser::GetNextToken(in, line);

    }

    Parser::PushBackToken(tok);

	return true;



}//End of ExprList

// 13. Expr ::= MultExpr { ( + | - | // ) MultExpr }
bool Expr(istream& in, int& line, Value & retVal ) {

    
    if (!MultExpr(in, line, retVal)) {
        return false;
    }
    LexItem t = Parser::GetNextToken(in, line);
    while (t == PLUS || t == MINUS || t == CAT) { // Assuming DIV_OP is the token for "//"
        Value nextVal;

        if (!MultExpr(in, line, nextVal)) {

            ParseError(line, "Error in MultExpr");
            return false;
        }  

        if (t == PLUS) {
            retVal = retVal + nextVal;

            if(retVal.GetType()==VERR){
                ParseError(7, "Illegal operand type for the operation.");
                return false;
                
            }
        } else if (t == MINUS) {
            retVal = retVal - nextVal;
        } else if (t == CAT) {
            retVal = retVal.Catenate(nextVal);
        }
        t = Parser::GetNextToken(in, line);

        // t = Parser::GetNextToken(in, line);
        // cout<<"P     "<<t<<endl;
    }
    Parser::PushBackToken(t);
    return true;
}

//End of Expr

// 12. RelExpr ::= Expr [ ( == | < | > ) Expr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
    if (!Expr(in, line, retVal)) {
        ParseError(line, "Error in Expr()");
        return false;
    }

    LexItem t = Parser::GetNextToken(in, line);

    if (t == EQ || t == GTHAN || t == LTHAN) {
        Value nextVal;
        if (!Expr(in, line, nextVal)) {
            ParseError(line, "Error in Expr()");
            return false;
        }
        if(t==EQ){
            retVal = retVal == nextVal;


        }else if (t==GTHAN){
          retVal = retVal > nextVal;

          if(retVal.GetType()==VERR){
            ParseError(line, "Illegal operand type for the operation.");
            return false;

          }
        }else if(t==LTHAN){

            retVal = retVal<nextVal;
        }


    } else {
        ParseError(line, "Expected relational operator after expression RealExpr");
        return false;
    }

    return true;

}

//End of RealExpr




// 18. Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr)
bool Factor(istream& in, int& line, int sign, Value & retVal) {
    LexItem t = Parser::GetNextToken(in, line);

     

    // Check if it's an IDENT, ICONST, RCONST, or SCONST
    if (t == IDENT) {

        
        // It's an identifier
        string varName = t.GetLexeme();

        // Check if the variable is defined
        if (SymTable.find(varName) == SymTable.end()) {
            ParseError(line, "Undefined variable '" + varName + "'");
            return false;
        }

      

        for (const auto& pair : TempsResults) {
            const string& tempVarName = pair.first;
            const Value& tempValue = pair.second;

            if (tempVarName == varName) {
                if(tempValue.IsInt()){
                  
                    retVal = Value(sign * tempValue.GetInt());

                    }else if(tempValue.IsReal()){
                        retVal = Value(sign * tempValue.GetReal());

                        }else{

                        retVal = tempValue;
                        }
            }
        }

        return true;
    } else if (t == ICONST) {
        // Convert the lexeme to an integer
        int varNameInt = std::stoi(t.GetLexeme());
        retVal = Value(sign * varNameInt);
        return true;
    } else if (t == RCONST) {
        // Convert the lexeme to a double
        double varNameReal = std::stod(t.GetLexeme());
        retVal = Value(sign * varNameReal);
        return true;
    } else if (t == SCONST) {

        // It's a string constant
        retVal = Value(t.GetLexeme());
        return true;
    } else if (t == LPAREN) {
        // Check if the expression inside the parenthesis is valid
        if (!Expr(in, line, retVal)) {
            ParseError(line, "Missing closing parenthesis in Factor.");
            return false;
        }

        // Apply the sign to the expression inside the parenthesis
        if (sign == -1) {
            // Negate the expression if the sign is negative
            if (retVal.IsInt()) {
                retVal.SetInt(-retVal.GetInt());
            } else if (retVal.IsReal()) {
                retVal.SetReal(-retVal.GetReal());
            } else {
                ParseError(line, "Cannot negate non-numeric value inside parentheses.");
                return false;
            }
        }

        // Check for the closing parenthesis
        t = Parser::GetNextToken(in, line);
        if (t != RPAREN) {
            ParseError(line, "Missing closing parenthesis in Factor.");
            return false;
        }

        return true;
    } else if (t == MINUS) {
        // If a minus sign is encountered, propagate the negative sign
        // by multiplying it with the 'sign' parameter and recursively parse the factor
        return Factor(in, line, -sign, retVal);
    } else {
        ParseError(line, "Invalid token in Factor.");
        return false;
    }
}

//End of Factor




// 17. Var ::= IDENT
bool Var(istream& in, int& line, LexItem & idtok) {
    LexItem t = Parser::GetNextToken(in, line);

    if (t != IDENT) {
        ParseError(line, "MISSING IDENT TOKEN");
        return false;
    }




    initvar = t.GetLexeme();

   
    // Check if the variable is already in SymTable
    if (SymTable.find(initvar) != SymTable.end()) {
       
             return true;

    }

    

    SymTable[initvar] = idtok.GetToken();

    varList.push_back({initvar, false});
    // Push variable name and its initialization status (initially false)

    return true;
    
}
//end of Var
