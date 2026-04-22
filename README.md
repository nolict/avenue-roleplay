# Avenue Roleplay

Avenue Roleplay is an open.mp roleplay gamemode built in Pawn. This project is structured to grow as a long-term RP codebase, with a clear module layout, account-based admin system, character flow, and internal documentation that keeps future development consistent.

## Table of Contents

- [Tech Stack](#tech-stack)
- [Quick Start](#quick-start)
- [Database Setup](#database-setup)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Credits](#credits)
- [License](#license)

## Tech Stack

- Language: Pawn
- Runtime: open.mp
- Build Tool: `sampctl`
- Database: MySQL
- Password Hashing: `samp-bcrypt`
- Command Parser: `Southclaws/zcmd`
- Standard Libraries:
  - `openmultiplayer/omp-stdlib`
  - `pawn-lang/samp-stdlib@open.mp`
  - `pawn-lang/pawn-stdlib@open.mp`

## Quick Start

```bash
git clone https://github.com/nolict/avenue-roleplay.git
cd avenue-roleplay
sampctl ensure
```

## Database Setup

```bash
cp gamemodes/modules/core/config/database.inc.example gamemodes/modules/core/config/database.inc
```

After that:

1. fill in your MySQL credentials in `gamemodes/modules/core/config/database.inc`
2. import `database/schema.sql`
3. build with `sampctl build`
4. run the server in your open.mp environment

## Documentation

Project documentation is split across:

- admin command reference: [docs/commands/admin.md](docs/commands/admin.md)

The admin command reference contains the current command list, required admin levels, short explanations, and usage examples.

## Contributing

Contributions are welcome as long as changes follow the current project structure and documentation rules.

Recommended workflow:

1. create a feature branch
2. keep changes scoped to the affected domain
3. update docs when behavior changes
4. run `sampctl build` before submitting changes
5. use clear commit messages

## Credits

- open.mp
- sampctl
- `pBlueG/SA-MP-MySQL`
- `Sreyas-Sreelal/samp-bcrypt`
- `Southclaws/zcmd`
- project contributors and maintainers of Avenue Roleplay

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
