### -1) Elementy składowe języka

- Komentarze single/multi
- Prymitywne typy danych
- zmienne
- kwalifikatory zmiennych (mutable, const, ref, cloned)
- operatory
- dyrektywy kontroli przepływu
- funkcje
- scope
- struktury danych
- wejscie/wyjscie
- słowa kluczowe

- generyczne typy i funkcje


### 0) język PROTON

Uruchomienie programu, wykonanie zaczyna się od funkcji 'main'

    proton main.prot
    

### 1) Założenia

- ogólnego użytku
- mutable domyślnie
- silnie typowany
- przekazywanie przez wartość
- obsługa referencji
- automatyczna inferencja typu
- łatwa obsługa stdin i stdout

```
a: int = 6; 			<-- mutable domyślnie
const a: int = 0; 		<-- const explicit
name := "Mikołaj"; 		<-- automatyczna inferencja typu ':='
ref myName: string = name;	<-- referencja
```

### 2) Podstawowe typy danych
```
    a: int
    b: float;
    c: byte;
    isRequired: bool;
    name: string;
    message: byte[];

    number: variant<int, float, string> = "6";
    const value: float := number.value(); 	<-- Error, variant holds type "string"!
    const value: string := number.value();	<-- OK
    
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
    id := myCredentials.id;
```

### 3) Funkcje

    Argumenty i typ zwracany domyślnie przez kopię
    ```
    fn login(c: credentials) -> void { }
    ```
    Domyślnie przez wartość
    ```
    fn add(c: int) -> int { }
    ```
    Referencja
    ```
    fn login(ref c: credentials) -> int { }
    ```
    const Referencja
    ```
    fn login(const ref c: credentials) -> int { }
    ```
    
    Zwracanie przez referencję
    ```
    fn get(ref pool: Pool) -> Obj ref { }
    ```


### 4) Keywords

    null

    int
    float
    bool
    byte
    string
    variant
    
    true
    false
    
    const
    ref
    fn
    type
    typeof
    return
    iter
    while

    generic <-- jeśli będzie czas
    any


### 5) Operatory

1) przypisania
```
=
:=
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
<
>
```

bitowe
```
!
&
|
^
<<
>>
```
```
<<<     <--- read from stdin
>>>     <-- write to stdout
```


### 6) Pętle for

    iter i ..5 {

    }

    iter i 3..5 {

    }

    iter countdown 11..1 {
    
    }

### 7) Pętle while

    iter while (true) {

    }
    
    i := 10;
    iter while (i > 0) {
        i = i - 1;
        <<< i;
    }


### 8) Wbudowana obsługa stdio:

    int x = >>> "Podaj liczbę:";
    result := x * i;
    <<< f"Poproszę {i * 5} tokenów";


### 9) Komentarze
```
$ One line comment
$ Multiline comment $
```

### 10) Obsługa błędów - komunikaty

```
ERROR in line 5:13 of /main.prot:

WHAT? >> Exception "Division by zero"!

TRACEBACK:

>> fn main() in line 10:3 of /main.prot
>> fn divide(int a, int b) in line 4:3 of /main.prot

    >>>> return a / 0; <<<< in line 5:13 of /main.prot
```
```
ERROR in line 5:13 of /main.prot:

WHAT? >> Syntax error "Assignment to unknown identifier"!

TRACEBACK:

>> fn main() in line 10:3 of /main.prot
>> fn divide(int a, int b) in line 4:3 of /main.prot

    >>>> result = 0; <<<< in line 5:13 of /main.prot <-- should be result := 0;
```

### 11) Dodatkowo jeśli będzie czas - generyczność

słowo kluczowe 'generic' + argument/member of type any

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

