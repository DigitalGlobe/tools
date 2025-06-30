#ifndef BISON_CMDLINE_GRAM_TAB_H
# define BISON_CMDLINE_GRAM_TAB_H

#ifndef YYSTYPE
typedef union {
    char *string;
    shpid_t pgno;
    int number;
    float flt;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	ID	257
# define	STRCONST	258
# define	INTCONST	259
# define	IOPARAM	260
# define	FLOATCONST	261
# define	CREATE	262
# define	INSERT	263
# define	OPEN	264
# define	CLOSE	265
# define	QUIT	266
# define	SELECT	267
# define	DELETE	268
# define	NL	269
# define	ERROR	270
# define	CHECK	271
# define	SPLIT	272
# define	HELP	273
# define	DUMP	274
# define	ONLY	275
# define	SET	276
# define	STRUCT	277
# define	WRITE	278
# define	ECHO_TOKEN	279
# define	EVALSPLIT	280
# define	EVALPENALTY	281
# define	LOADFILE	282
# define	PREDINFO	283
# define	CREATEANL	284
# define	OPENANL	285
# define	CLOSEANL	286
# define	WKLDSTATS	287
# define	SPLITSTATS	288
# define	PENALTYSTATS	289
# define	ANLINFO	290


extern YYSTYPE yylval;

#endif /* not BISON_CMDLINE_GRAM_TAB_H */
