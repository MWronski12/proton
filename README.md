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

    Uruchomienie programu, wykonanie zaczyna się od funkcji 'main'

    proton main.prot
    

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
    a: int
    b: float;
    c: byte;
    isRequired: bool;
    name: string;

    number: variant<int, float, string> = "22";

    if (number holds string) {
        whatsMyAge: string = "I am " + number as string + "years old.";
        <<< whatsMyAge;
    }

    const value: float = number as float; 	<-- Error, variant holds type "string"!
    const value: string = number as string;	<-- OK
    
    type Credentials {
        id: int;
        username: string;
        password: string;
    };                                          <-- Musi być srednik po deklaracji typu

    type simple { int a } = { 1 };              <-- Niepoprawne, trzeba najpierw zadeklarować typ, a następnie stworzyć zmienną tego typu;

    const myCredentials: Credentials = {        <-- Przypisywać
        1, 
        "root", 
        "password"
    };     

    id: int = credentials.id;
```

### 3) Funkcje

    Argumenty i typ zwracany domyślnie przez kopię
    ```
    fn login(c: Credentials) -> void { }
    ```
    
    ```
    fn helloWorld(void) -> void { }
    ```

### 4) If statements

    ```
    if (true) {

    }
    ```

    ```
    if (a < 0) {

    } elif (a == 0) {

    } else {

    }
    ```

    ```
    if (isDigit(n)) {

    } else {

    }
    ```


### 5) Keywords

    null
    void
    int
    float
    bool
    byte
    string
    variant
    true
    false
    type
    
    fn
    return
    iter
    while
    if
    elif
    else

    // jeśli będzie czas:
    generic
    any
    typeof


### 6) Operatory

    przypisania
    ```
    =
    ```

    arytmetyczne
    ```
    +
    -
    *
    /
    %
    ```

    logiczne
    ```
    !
    &&
    ||
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

    bitowe
    ```
    ~
    &
    |
    ^
    ```


### 7) Pętle for

    iter i ..5 {

    }

    iter i 3..5 {

    }

    iter i 11..1 {
    
    }

    int a = 0;
    iter i a..10 {

    }

    int range = 10;
    iter i startVal()..startVal() + range {

    }

    Nie trzeba podawać typu, zawsze jest to int. Jeśli zmienna już istnieje, to jest przysłaniana.

### 8) Pętle while

    iter while (true) {

    }
    
    i := 10;
    iter while (i > 0) {
        i = i - 1;
        <<< i;
    }


### 9) Wbudowana obsługa stdio:

    int x = >>> "Podaj liczbę:";
    result := x * i;
    <<< "Poproszę " <<< (i * 5) as string <<< tokenów";


### 10) Komentarze
    ```
    $ One line comment
    $ Multiline comment $
    ```

### 11) Scope

Na poziomie globalnym mogą istnieć tylko deklaracje funkcji.
Zienne z są przykrywane zmiennymi z głębszych scope'ów.

fn bar() {

    x: float = 1.0;

    if (true) {
        x: int = 5;
    } else {
        x: float = 6.0;
    }

    <<< x;
}

### 12) Obsługa błędów - komunikaty

    ```
    ERROR in line 5:13 of /main.prot:

    WHAT? >> Exception "Division by zero"!

    TRACEBACK:

    >> fn main(void) in line 10:3 of /main.prot
    >> fn divide(int a, int b) in line 4:3 of /main.prot

        >>>> return a / 0; <<<< in line 5:13 of /main.prot
    ```

    ```
    ERROR in line 5:13 of /main.prot:

    WHAT? >> Syntax error "Assignment to unknown identifier"!

    TRACEBACK:

    >> fn main(void) in line 10:3 of /main.prot
    >> fn divide(int a, int b) in line 4:3 of /main.prot

        >>>> result = 0; <<<< in line 5:13 of /main.prot <-- should be result := 0;
    ```

13) Sposób testowania

Na początek przygotować złożone przykłady i ręcznie przetestować poprawność gramatyki.

Dla każdego rodzaju tokenu sprawdzić czy lexer go poprawnie przetwarza.

Dla parsera trzeba przetestować każdą kombinację gałęzi (alternatywy z produkcji gramatyki).

Wygenerować lexer oraz parser za pomocą definicji gramatyki, a następnie go przetestować jak powyżej.
