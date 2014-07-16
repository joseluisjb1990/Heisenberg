\<programa\> ::= \<definiciones\> **oso** **(** **)** **=>** **extinto** \<bloque\>

\<definiciones\> ::= { \<listadefglobales\> }

\<listadefglobales\> ::= \<definicionglobal\> { \<definicionglobal\> }

\<definicionglobal\> ::= \<defconstante\> | \<defvariable\> | \<deffuncion\> | \<defcompleja\>

\<deffuncion\> ::= **id** **(** \<defparametros\> **)** **=>** \<tipo\> **;** | **id** **(** \<defparametros\> **)** =\> \<tipo\> \<bloque\>

\<defparametros\> ::= \<defparametro\> { **,** \<defparametro\> }

\<defparametro\> ::= \<tipo\> **id** | **^** \<tipo\> **id** | \<tipocueva\> **id**

\<tipocueva\> ::= \<parametrocueva\> \<tipo\>

\<parametrocueva\> ::= **cueva** **[** **]** **de** | \<parametrocueva\> **cueva** **[** **constpolar** **]** **de**

\<defconstante\> ::= **const** \<tipo\> \<identificadores\> **=** \<expresiones\> **;**

\<defvariable\> ::= \<tipo\> \<identificadores\> **=** \<expresiones\> **;** | \<tipo\> \<identificadores\> **;** | \<defcueva\> **id** **;**

\<identificadores\> ::= **id** { **,** **id** }

\<defcueva\> ::= \<cuevas\> \<tipo\>

\<cuevas\> ::= **cueva** **[** **constpolar** **]** **de** { **cueva** **[** **constpolar** **]** **de** }

\<defcompleja\> ::= **pardo** **id** **{** \<campos\> **}** | **pardo** **id** **;** | **grizzli** **id** **{** \<campos\> **}** | **grizzli** **id** **;**

\<campos\> ::= **campo** **;** { **campo** **;** }

\<campo\> ::= \<tipo\> **id** | \<defcueva\> **id**

\<tipo\> ::= **panda** | **polar** | **kodiak** | **malayo** | **hormiguero** | **extinto** | **id**

\<bloque\> ::= **{** \<instrucciones\> **}** | \<instruccion\> **;**

\<instrucciones\> ::= \<instruccion\> { \<instruccion\> }

\<instruccion\> ::= \<defvariable\>
                  | \<defconstante\>
                  | \<lvalues\> **=** \<expresiones\> **;**
                  | **leer** **(** **id** **)** **;**
                  | **escribir** **(** \<expresion\> **)** **;**
                  | **id** **(** \<expresiones\> **)** **;**
                  | **si** \<expresion\> **entonces** \<bloque\> [ **sino** \<bloque\> ]
                  | **para** **id** **en** **(** \<expresion\> [ **;** \<expresion\> ] **;** \<expresion\> **)** \<bloque\>
                  | **para** **id** **en** **id** \<bloque\>
                  | \<iteracionindeterminada\>
                  | **id** **++** **;**
                  | **id** **--** **;**
                  | **vomita** [ **id** ] **;**
                  | **fondoblanco** [ **id** ] **;**
                  | **roloepea** [ **id** ] **;**

\<iteracionindeterminada\> ::= [ **id** **:** ] **mientras** **(** \<expresion\> **)** \<bloque\>

\<lvalues\> ::= \<lvalue\> { **,** \<lvalue\> }

\<lvalue\> ::= **id** [ \<maybecueva\> ] { **->** **id** [ \<maybecueva\> ] | **.** **id** [ \<maybecueva\> ] }

\<maybecueva\> ::= \<accesocueva\>

\<accesocueva\> ::= **[** \<expresion\> **]** { **[** \<expresion\> **]** }

\<expresiones\> ::= \<expresion\> { **,** \<expresion\> }

\<expresion\> ::= **constpolar**
                | **constkodiak**
                | **consthormiguero**
                | **constmalayo**
                | \<lvalue\>
                | **id** **(** \<expresiones\> **)**
                | \<funcionpredef\>
                | **blanco**
                | **negro**
                | \<expresion\> **<** \<expresion\>
                | \<expresion\> **=<**  \<expresion\>
                | \<expresion\> **>** \<expresion\>
                | \<expresion\> **>=**  \<expresion\>
                | \<expresion\> **==**  \<expresion\>
                | \<expresion\> **=/=** \<expresion\>
                | \<expresion\> **|** \<expresion\>
                | \<expresion\> **&** \<expresion\>
                | **no** \<expresion\>
                | \<expresion\> **+** \<expresion\>
                | \<expresion\> **-** \<expresion\>
                | \<expresion\> **\*\*** \<expresion\>
                | \<expresion\> **\*** \<expresion\>
                | \<expresion\> **/** \<expresion\>
                | \<expresion\> **%** \<expresion\>
                | **-** \<expresion\>
                | **(** \<expresion\> **)**
                | \<expresion\> **?** \<expresion\> **:** \<expresion\>

\<funcionpredef\> ::= **a_panda** **(** \<expresion\> **)** | **a_kodiak** **(** \<expresion\> **)** | **a_malayo** **(** \<expresion\> **)** | **a_polar** **(** \<expresion\> **)** | **lon** **(** \<expresion\> **)**
