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
    a = "6";                <-- invalid
    >>>                     <-- read from stdin
    <<<                     <-- write to stdout
    ```

    "<<<" i ">>>" będę traktował jako wbudowane funkcje, a nie słowa kluczowe ani operatory.

### 2) Podstawowe typy danych
```
    a: int
    b: float;
    c: byte;
    isRequired: bool;
    name: string;
    message: byte[]; <-- tablice to listy dynamiczne

    number: variant<int, float, string> = "6";
    const value: float = number.value; 	<-- Error, variant holds type "string"!
    const value: string = number.value;	<-- OK
    
    type Credentials {
        id: int;
        string username;
        string password;
    }

    credentials: Credentials = {
        id: 1,
        username: "root",
        password: "password",
    };

    const myCredentials: Credentials = {1, "root", "password"};

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

4) If statements

    ```
    if true {

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

    1) przypisania
    ```
    =
    ```

    2) arytmetyczne dwuargumentowe
    ```
    +
    -
    *
    /
    %
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
    !
    &
    |
    ^
    ```


### 7) Pętle for

    iter i ..5 {

    }

    iter i 3..5 {

    }

    iter countdown 11..1 {
    
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
    <<< f"Poproszę {i * 5} tokenów";


### 10) Komentarze
    ```
    $ One line comment
    $ Multiline comment $
    ```

### 11) Scope

    Wyróżniamy zmienne lokalne i globalne.

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

### 13) Dodatkowo jeśli będzie czas - generyczność, auto, referencje

    opcjonalne słowo kluczowe 'generic' dla czytelności + argument/member of type any:

        argumenty funkcji
        ```
        generic fn increment(ref number: any) -> void {}
        ```

        pola typów
        ```
        generic type Container {
            size: int;
            elements: any[];
        }
        ```

    Automatyczna inferencja typu

        ```
        a := 5;
        b := "Hej";
        ```

    Obsługa referencji

        ```
        a := 5;
        ref a_ref = a;

        fn increment(ref x: int) -> void { x = x + 1; }

        fn getInstance(void) -> Singleton ref { return instance; }
        ```

14) Sposób testowania

Wygeneruję lexer oraz parser za pomocą definicji gramatyki, a następnie korzystając z powyższych przykładów poprawnych oraz osobno przygotowanych przykładów niepoprawnych, będę testował czy parser prawidłowo obsługuję daną kontrukcję.
