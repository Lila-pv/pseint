' // * Este ejemplo es para observar las estructuras condicionales y
' // repetitivas m�s simples

  	c=0
	WHILE c <= 10
      c = c + 1
	  PRINT c
    WEND
	PRINT "-"

	c=0
	DO
      c = c + 1
	  PRINT c
    LOOP UNTIL c = 10
	PRINT "-"

	c=0
	DO
      c = c + 1
	  PRINT c
    LOOP WHILE c < 10
	PRINT "-"

 	IF C=10 THEN PRINT "Si" '// Si el bloque de instrucciones de una instrucci�n IF s�lo va a llevar una instrucci�n se codifica en una sola linea

	IF C=10 THEN
		PRINT "Si"
	ELSE
		PRINT "NO"
	END

