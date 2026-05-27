# strgen

A minimal command-line password generator written in C. Uses `getrandom(2)` for cryptographically secure randomness and Fisher-Yates shuffle for distribution.

## Build

```sh
make        # build → bin/strgen
make run    # build and run with no flags (default output)
make clean  # remove obj/ and bin/
```

## Usage

```
./bin/strgen [-l length] [-c count] [-d delimiter] [-g classes] [-x chars]
```

## Options

| Flag | Argument | Description |
|------|----------|-------------|
| `-l` | `<n>` | Password length (default: `12`) |
| `-c` | `<n>` | Number of passwords to generate (default: `1`) |
| `-d` | `<str>` | Delimiter printed between passwords (default: `-`) |
| `-g` | `<classes>` | Comma-separated character classes. Disables all classes first, then enables only those listed. |
| `-x` | `<chars>` | Characters to explicitly exclude |

### Character Classes

Used with `-g`. Valid values:

- `lower` — a–z
- `upper` — A–Z
- `digit` — 0–9
- `punct` — printable punctuation

## Examples

Default — one 12-character password, all classes:

```sh
./bin/strgen
```

Apple Passwords-style — 6 characters, 3 groups, lowercase/uppercase/digit, hyphen-delimited:

```sh
./bin/strgen -l 6 -c 3 -g lower,upper,digit
# DN96gc-KP0rj2-1dTX6v
```

Generate a 20-character password:

```sh
./bin/strgen -l 20
```

4 passwords separated by newlines:

```sh
./bin/strgen -c 4 -d $'\n'
```

Digits only, 2 passwords, custom delimiter, excluding `1`, `2`, `3`:

```sh
./bin/strgen -g digit -c 2 -d " 00 " -x "123"
```

## Notes

- If `-g` is not specified, all four character classes are enabled.
- `-g` resets all classes before applying the listed ones — `-g lower,upper` enables only those two.
- Each character position cycles through the enabled classes before shuffling, ensuring class representation.
- If a position exhausts the random buffer without finding a valid character, it returns a null byte. If this happens, consider narrowing your exclusion list or changing classes.
- The exclusion list is capped at 32 characters. To exclude broader sets, disable the class entirely with `-g`.

## Security

`strgen` reads from the kernel entropy pool via `getrandom(2)` with no flags, blocking until sufficient entropy is available — equivalent to `/dev/urandom` post-initialization. Passwords are shuffled with Fisher-Yates using one random byte per swap position.
