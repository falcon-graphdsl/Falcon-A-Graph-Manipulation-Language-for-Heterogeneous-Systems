/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    I_CONSTANT = 259,
    B_CONSTANT = 260,
    F_CONSTANT = 261,
    STRING_LITERAL = 262,
    FUNC_NAME = 263,
    SIZEOF = 264,
    GETTYPE = 265,
    WHITE_SPACE = 266,
    SECTION = 267,
    SECTIONS = 268,
    PARALLEL = 269,
    PTR_OP = 270,
    INC_OP = 271,
    DEC_OP = 272,
    LEFT_OP = 273,
    RIGHT_OP = 274,
    LE_OP = 275,
    GE_OP = 276,
    EQ_OP = 277,
    NE_OP = 278,
    AND_OP = 279,
    OR_OP = 280,
    MUL_ASSIGN = 281,
    DIV_ASSIGN = 282,
    MOD_ASSIGN = 283,
    ADD_ASSIGN = 284,
    RADD_ASSIGN = 285,
    RMUL_ASSIGN = 286,
    SUB_ASSIGN = 287,
    LEFT_ASSIGN = 288,
    RIGHT_ASSIGN = 289,
    AND_ASSIGN = 290,
    XOR_ASSIGN = 291,
    OR_ASSIGN = 292,
    TYPEDEF_NAME = 293,
    ENUMERATION_CONSTANT = 294,
    TYPEDEF = 295,
    EXTERN = 296,
    STATIC = 297,
    AUTO = 298,
    REGISTER = 299,
    INLINE = 300,
    CONST = 301,
    RESTRICT = 302,
    VOLATILE = 303,
    BOOL = 304,
    CHAR = 305,
    SHORT = 306,
    INT = 307,
    LONG = 308,
    SIGNED = 309,
    UNSIGNED = 310,
    FLOAT = 311,
    DOUBLE = 312,
    VOID = 313,
    SINGLE = 314,
    COMPLEX = 315,
    IMAGINARY = 316,
    STRUCT = 317,
    UNION = 318,
    ENUM = 319,
    ELLIPSIS = 320,
    STRUCT_REC = 321,
    CASE = 322,
    DEFAULT = 323,
    IF = 324,
    ELSE = 325,
    SWITCH = 326,
    WHILE = 327,
    DO = 328,
    FOR = 329,
    GOTO = 330,
    CONTINUE = 331,
    BREAK = 332,
    RETURN = 333,
    HASH_DEFINE = 334,
    ALIGNAS = 335,
    ALIGNOF = 336,
    ATOMIC = 337,
    GENERIC = 338,
    NORETURN = 339,
    STATIC_ASSERT = 340,
    THREAD_LOCAL = 341,
    GRAPH = 342,
    EDGE = 343,
    SET = 344,
    COLLECTION = 345,
    POINT = 346,
    E_P = 347,
    G_P = 348,
    P_P = 349,
    GPU = 350,
    POINTS = 351,
    EDGES = 352,
    NBRS = 353,
    INNBRS = 354,
    OUTNBRS = 355,
    ITEMS = 356,
    FOREACH = 357,
    INOLD = 358,
    ATOMIC1 = 359,
    EDGE1 = 360,
    POINT1 = 361,
    DIR_DECL = 362,
    MACRO1 = 363,
    GETTYPE1 = 364,
    PR_EXPR1 = 365,
    THEN = 366
  };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define I_CONSTANT 259
#define B_CONSTANT 260
#define F_CONSTANT 261
#define STRING_LITERAL 262
#define FUNC_NAME 263
#define SIZEOF 264
#define GETTYPE 265
#define WHITE_SPACE 266
#define SECTION 267
#define SECTIONS 268
#define PARALLEL 269
#define PTR_OP 270
#define INC_OP 271
#define DEC_OP 272
#define LEFT_OP 273
#define RIGHT_OP 274
#define LE_OP 275
#define GE_OP 276
#define EQ_OP 277
#define NE_OP 278
#define AND_OP 279
#define OR_OP 280
#define MUL_ASSIGN 281
#define DIV_ASSIGN 282
#define MOD_ASSIGN 283
#define ADD_ASSIGN 284
#define RADD_ASSIGN 285
#define RMUL_ASSIGN 286
#define SUB_ASSIGN 287
#define LEFT_ASSIGN 288
#define RIGHT_ASSIGN 289
#define AND_ASSIGN 290
#define XOR_ASSIGN 291
#define OR_ASSIGN 292
#define TYPEDEF_NAME 293
#define ENUMERATION_CONSTANT 294
#define TYPEDEF 295
#define EXTERN 296
#define STATIC 297
#define AUTO 298
#define REGISTER 299
#define INLINE 300
#define CONST 301
#define RESTRICT 302
#define VOLATILE 303
#define BOOL 304
#define CHAR 305
#define SHORT 306
#define INT 307
#define LONG 308
#define SIGNED 309
#define UNSIGNED 310
#define FLOAT 311
#define DOUBLE 312
#define VOID 313
#define SINGLE 314
#define COMPLEX 315
#define IMAGINARY 316
#define STRUCT 317
#define UNION 318
#define ENUM 319
#define ELLIPSIS 320
#define STRUCT_REC 321
#define CASE 322
#define DEFAULT 323
#define IF 324
#define ELSE 325
#define SWITCH 326
#define WHILE 327
#define DO 328
#define FOR 329
#define GOTO 330
#define CONTINUE 331
#define BREAK 332
#define RETURN 333
#define HASH_DEFINE 334
#define ALIGNAS 335
#define ALIGNOF 336
#define ATOMIC 337
#define GENERIC 338
#define NORETURN 339
#define STATIC_ASSERT 340
#define THREAD_LOCAL 341
#define GRAPH 342
#define EDGE 343
#define SET 344
#define COLLECTION 345
#define POINT 346
#define E_P 347
#define G_P 348
#define P_P 349
#define GPU 350
#define POINTS 351
#define EDGES 352
#define NBRS 353
#define INNBRS 354
#define OUTNBRS 355
#define ITEMS 356
#define FOREACH 357
#define INOLD 358
#define ATOMIC1 359
#define EDGE1 360
#define POINT1 361
#define DIR_DECL 362
#define MACRO1 363
#define GETTYPE1 364
#define PR_EXPR1 365
#define THEN 366

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 80 "grammar.y" /* yacc.c:1909  */

    int  info;
    long ival;
    double fval;
    char* text;
    bool bval;
class  tree_node* ptr;
   // expr_list* e_list;  // list of expressions
    //lhs_list* l_list;   // list of lhs
    //struct parse_temp_t pair;

#line 288 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
