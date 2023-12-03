### -1) Elementy składowe języka

    - Komentarze single/multi
    - Prymitywne typy danych
    - zmienne
    - operatory
    - dyrektywy kontroli przepływu
    - funkcje
    - scope
    - struktury danych
    - wejscie/wyjscie
    - słowa kluczowe


    Jeśli będzie czas chciałbym dodać:
        - generyczne typy i funkcje (argument lub member typu "any")
        - automatyczną inferencję typów przy pomocy ":="
        - obsługę zmiennych, argumentów funckji i wartości zwracanych jako referencje


### 0) język PROTON

    Uruchomienie programu "proton"

    - Pierwszym argumentem jest plik z kodem źródłowym
    - Następnie opcjonalnie można podać dowolną liczbę argumentów oddzielonych znakiem białym, które zostaną przypisane do globalnych zmiennych typu string o identyfikatorze 'ARG_<num>' (ARG_1, ARG_2...)

    proton main.prot 5 identifier

    ---- w kodzie źródłowym ----

    fn main() {
        arg1: string = ARG_1;
        arg2: string = ARG_2;

        $ arg1: int = ARG_1;        <-- INVALID, cannot assign type string to variable of type int!

        << "Argument 'number' is " << arg1 << "\n";
        << "Argument 'id' is " << arg2 << "\n";  
    }
    

### 1) Założenia

    - ogólnego użytku
    - mutable domyślnie
    - silnie typowany
    - przekazywanie przez wartość
    - łatwa obsługa stdin i stdout

    ```
    a: int = 6; 			<-- mutable domyślnie
    const a: int = 0; 		<-- const explicit
    a = "6";                <-- invalid - a is const
    ```

### 2) Podstawowe typy danych
```
    a: int;
    b: float;
    c: char;
    isRequired: bool;
    name: string;

    a = 5;
    b = 6.6;

    c = 'C';    <-- chary są zawsze definiowane przy użyciu pojedynczych nawiasów!

    name = "Mikołaj";   <-- stringi są zawsze definiowane przy użyciu podwójnych nawiasów!
    isRequired = true;
```

### 3) Rekord wariantowy

    Rekord variantowy pozwala na przechowywanie przez jedną zmienną różnych typów, co domyślnie jest niedozwolone w języku silnie typowanym.

    ```
    // Deklaracja typu wariantowego 'Number'
    variant Number {                            
        int;
        float;
        string;
    };                                          <-- Musi być srednik po deklaracji typu (variant jest traktowany jak typ użytkownika)

    number: Number = Number{ "22" };            <-- Trzeba używać nazwy typu explicit przy przypisywaniu wartości


    // Sprawdzanie typu przetrzymywanego przez variant 'Number' za pomocą konstrukcji match case
    match number {
        case int -> {
            << "Variable 'number' of type 'Number' currently holds int and it's value is"
            << number as int
            << "\n";
        }
        case float -> {
            << "Variable 'number' of type 'Number' currently holds float and it's value is"
            << number as float
            << "\n";
        }
        case string -> {
            << "Variable 'number' of type 'Number' currently holds string and it's value is"
            << number as string 
            << "\n";
        }
    }


    // Wypakowanie wartości przetrzymywanej przez variant
    const value: float = number as float; 	    <-- Error, variant holds type "string"!
    const value: string = number as string;	    <-- OK


    // typ variantowy, oprócz typów prymitywnych, może przetrzymywać również typy użytkownika
    struct Dog {
        name: string;
        likesRunning: bool;
    };

    struct Cat {
        name: string;
        likesMilk: bool;
    };

    struct Bison {
        weight: float;
        age: int;
    };

    variant Animal {
       Dog;
       Cat;
       Bison;
       string;
    };

    animal: Animal;

    animal = Dog{ name: "Burek", likesRunning: true };
    animal = Cat{ name: "Kitek", likesMilk: true };
    animal = "UNIDENTIFIED ANIMAL";

    match animal {
        case Dog -> { << "It's a dog named " << (animal as Dog).name << "!"; }
        case Cat -> { 
            unpackedCat: Cat = animal as Cat;

            << "It's a cat named "
            << unpackedCat.name
            << "!\n";
        }
        case Bison -> { << "It's a bison!\n"; }
        case string -> { << "It's an " << animal as string << "!\n"; }
    }
    ```

### 4) typy/struktury użytkownika

    ```
    struct Credentials {
        id: int;
        username: string;
        password: string;
    };                                          <-- Musi być srednik po deklaracji typu

    // Definiowanie zmiennej typu użytkownika
    const rootCredentials: Credentials = Credentials{
        id: 1,
        usename: "root",
        password: "password",
    };

    const myCredentials: Credentials = Credentials{
        1, "root", "password"                   <-- Można pominąć identyfikatory
    };     


    // Podawanie nazwy konstruktora jest obowiązkowe przy przypisywaniu wartości do struktury, w celu uniknięcia niejedneznoczności.

    variant VariantCredentials {
        Credentials;
        HashedCredentials;
    };

    credentials: VariantCredentials;
    credentials = Credentials{ 2, "admin", "proAdminPass%^&" };
    credentials = HashedCredentials{ 3, "anonymus666", "1^dg#578DXaz$%zaq!23Sa" };

    match creds {
        case Credentials -> { << "My credentials are not secured!\n" }
        case HashedCredentials -> { << "My credentials are secured!\n" }
    }


    // Odwołanie do pola struktury
    credentials: Credentials = Credentials{ 4, "Moe", "moe123" };

    id: int = credentials.id;
    username: string = credentials.username;

    credentials.password = "hArd#rpAssw0rd$%";
    ```


### 5) Funkcje

    Argumenty i wartość zwracana to kopie
    ```
    fn login(c: Credentials) -> void { return; }
    ```
    
    Jeśli nie określimy argumentów oraz typu zwracanego, domyślnie są to "void"
    ```
    fn helloWorld() { return; }
    ```

    Argumenty mogą być const
    ```
    fn printCredentials(const credentials: Credentials) { ... }
    ```

    Funkcje mogą być definiowane wewnątrz innych funkcji, przyjmowane jako argument oraz zwracane przez inne funkcje
    ```
    fn decorator(func: () -> int) -> () -> int {
        fn decorated() -> int {
            << "This is a decorated function call!\n";
            return func();
        }

        return decorated;
    }
    ```

    // Funkcje mogą przyjmować typy użytkownika jako argumenty oraz je zwracać
    ```
    struct Point {
        x: float;
        y: float;
    };

    fn calculateDistance(A: Point, B: Point) -> float {
        $ Calculate distance here
        distance: float = 0;

        return distance;
    }
    ```

    ```
    struct Person {
        name: string;
        age: int;
    };

    fn foo() -> Person {
        me: Person = Person{ "Mikołaj", 22 };

        << "My name is " << me.name << "\n";
        << "I am " << me.age << "years old\n";

        return me;
    }
    ```

### 6) If statements

    ```
    if true {

    }
    ```

    ```
    if a < 0 {

    } elif a == 0 {

    } else {

    }
    ```

    ```
    if isDigit(n) {

    } else {

    }
    ```


### 7) Keywords

    void
    int
    float
    bool
    char

    string
    variant
    struct

    true
    false
    struct
    
    fn
    return
    if
    elif
    else
    for
    in
    until
    while
    continue
    break

    match
    case
    as

### 8) Operatory

    ```
    =       - przypisanie
    ()      - wywołanie
    .       - dostęp do pola obiektu
    as      - dostęp do wartości wariantu (np.: variantVar as float)
    type()  - castowanie (np.: float(intVar))
    
    ```

    arytmetyczne
    ```
    +
    -
    *
    /
    %
    ```

    relacyjne
    ```
    ==
    !=
    <
    >
    <=
    >=
    ```

    logiczne
    ```
    ||
    &&
    !
    ```

#### Priorytety i łączność operatorów

    Priorytet 0 -   łączność od lewej do prawej:

        ||      -   logical or

    Priorytet 1 -   łączność od lewej do prawej:

        &&      -   logical and

    Priorytet 2 - łączność od lewej do prawej:

        ==      -   equal to
        !=      -   not equal to

    Priorytet 3 - łączność od lewej do prawej:

        <       -   mniejszy
        <=      -   mniejszy równy
        >       -   większy
        >=      -   większy równy

    Priorytet 4 - łączność od lewej do prawej:

        +       -   dodawanie
        -       -   odejmowanie 

    Priorytet 5 - łączność od lewej do prawej:

        *       -   mnożenie
        /       -   dzielenie 
        %       -   modulo

    Priorytet 6 - łączność od prawej do lewej:

        !       -   negacja logiczna
        -       -   negacja arytmetyczna
        
    Priorytet 7 - łączność od lewej do prawej:
        .       -   dostęp do pola struktury
        ()      -   wywołanie funkcji
        as      -   wypakowanie wartości variantu
        type()  -   castowanie (np.: float(intVar))


### 9) Pętle for

    Zasięg to kolejne po sobie liczby całkowite, które reprezentujemy za pomocą zapisu "x until y".
    Klasycznie, początek zasięgu będzie brany pod uwagę, a koniec nie.

    Słowo kluczowe "in" służy do iterowania po zasięgu.
    Jest ono używane zamiast tradycyjnego podejścia do pętli for. 
    Zamiast "for (int i = 0; i < 10; i++) {}" piszemy "for i in 0 until 10".

    Typ zmiennej pętli jest automatycznie dedukowany na podstawie typu obiektu, po którym iterujemy. Narazie ten typ może być tylko zasięgiem liczb całkowitych definiowanym w postaci "x until y". Wraz z rozwojem języka można zaimplementować iterację po dowolnym iterowalnym obiekcie, takim jak string, tablica lub klasa implementująca odpowiednie metody.

    ```
    for i in 1 until 5 {

    }

    for i in 3 until 5 {

    }

    // Zmienna może się nazywać dowolnie, a zasięg może być zasięgiem malejącym
    countdown in 10 until 1 {

    }

    i: int = 10;
    for i in 11 until 1 {
        // Tutaj i z wyższego scope'u zostało przysłonięte
    }

    a: int = 0;
    for i in a until 10 {

    }

    fn startVal() -> int { return 5; }
    range: int = 10;
    for i in startVal() until startVal() + range {

    }

    name: string = "Mikołaj";
    length: int = 0;

    for letter in name {
        length = length + 1;
    }
    ```
 

### 10) Pętle while

    while true {

    }

    // We can use continue and break statements
    i: int = 0;
    while true {
        if i % 10 == 0 {
            continue;
        }

        if i == 100 {
            break;
        }

        i = i + 1;
    }
    
    i: int = 10;
    while i > 0 {
        i = i - 1;
        << i as string << '\n';
    }


### 11) Wbudowana obsługa stdio:

    Operator "<<":
        - służy do wypisywania wartości na stdout
        - może być wielokrotnie łączony
        - Obsługuje:
    
            * float, int    - wypisuje postac tekstowa liczby
            * char          - wypisuje znak ascii
            * string        - wypisuje łańcuch znaków
            * bool          - wypisuje "0" lub "1"

    ```
    x: int;
    x << "Podaj liczbę: ";  <-- wpisujemy do konsoli "10<ENTER>"

    << Twoja liczba " << x << " pomnożona przez 10 daje " << x * 10 << "\n";

    Wypisze na stdout "Twoja liczba 10 pomnożona przez 10 daje 100<NEWLINE>"
    ```

    Operator ">>":
        - służy do wczytywania wartości z stdin
        - po prawej stronie musi się znaleźć zmienna, do której przypiszemy wczytaną warość
        - W zależności od typu zmiennej, do której przypisujemy zczytuje wartości w różny sposób. Działa tak samo jak scanf, tylko zamiast implicit podawać jaki typ wczytujemy (scanf("%d", &a)), ten typ jest automatycznie dedukowany na podstawie zmiennej, do której przypisujemy wynik (a: int; >> a;).
        - Obsługuje tylko typy: int, float, char, string. Kolejne znaki z konsoli są wczytywane aż do momentu napotkania znaku niepasującego do regexa definiującego literał danego typu.


    ```
    a: int;
    << "Podaj liczbę całkowitą: ";
    >> a;

    b: float;
    << "Podaj liczbę zmiennoprzecinkową: ";
    >> b;

    c: char;
    << "Podaj jeden znak: ";
    >> c;

    name: string;
    << "Jak się nazywasz? ";
    >> name;
    ```

### 12) Komentarze
    ```
    $ One line comment

    $$
    Multiline
    comment
    $$
    ```

### 13) Scope i struktura programu

    Program składa się z 4 kluczowych sekcji, które muszą wystąpić kolejno po sobie:

        1) Sekcja deklaracji globalnych typów użytkownika (opcjonalna)
        2) Sekcja zmiennych globalnych (opcjonalna)
        3) Sekcja deklaracji funkcji użytkownika (opcjonalna)
        4) Funckja o identyfikatorze 'main' będąca punktem wejściowym programu

    Zmienne mogą zostać przykryte zmiennymi z niższych scope'ów, lecz na tym samym poziomie nie możemy ponownie zadeklarować zmiennej o tej samej nazwie.

    ```
    fn bar() {

        x: float = 1.0;

        if true {
            x: int = 5;
            << "Here x is int 5";
        } else {
            x: float = 6.0;
            << "Here x is float 6.0";
        }

        << "Here x is float 1.0";

        $ x: int = 10;      <-- ERROR, variable "x" was already declared in this scope!
    }
    ```

### 14) Castowanie

    ```
    a: int = 5;

    b: float = float(a);

    age: string = string(a);

    c: char = char(a);

    d: int = int(3.14);
    ```

### 15) Obsługa błędów - komunikaty

    Whenever interpreter encounters an error it stops its execution and prints an error message that is composed of position of the code which caused the error, explanation what kind of error it is and traceback of calls.

    ```
    ERROR in line 5:13 of /main.prot:

    WHAT? >>> Exception: "Division by zero"!

    TRACEBACK:

    >>> fn main(void) in line 10:3 of /main.prot
    >>> fn divide(int a, int b) in line 4:3 of /main.prot

        >>> "return a / 0;" <<< in line 5:13 of /main.prot
    ```

    ```
    ERROR in line 5:4 of /main.prot:

    WHAT? >>> Unknown identifier: "Variable 'result' was not declared in this scope!"!

    TRACEBACK:

    >>> fn main(void) in line 10:3 of /main.prot
    >>> fn divide(int a, int b) in line 4:3 of /main.prot

        >>> "result = 0;" <<< in line 5:4 of /main.prot
    ```

    ```
    ERROR in line 6:4 of /main.prot:

    WHAT? >>> Syntax error: "';' expected!"

    TRACEBACK:

    >>> fn main(void) in line 10:3 of /main.prot
    >>> fn printLine() in line 4:3 of /main.prot

        >>> ">> result;" <<< in line 6:4 of /main.prot        <-- Interpreter encountered next statement instead of semicolon
    ```

### 16) Sposób testowania

Na początek przygotować złożone przykłady i ręcznie przetestować poprawność gramatyki.

Dla każdego rodzaju tokenu sprawdzić czy lexer go poprawnie przetwarza.

Dla parsera trzeba przetestować każdą kombinację gałęzi (alternatywy z produkcji gramatyki).

Wygenerować lexer oraz parser za pomocą definicji gramatyki, a następnie go przetestować jak powyżej.
