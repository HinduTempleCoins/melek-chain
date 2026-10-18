# melek-chain

MELEK is a Graphene-family social blockchain — a fork of BLURT (which forked Steem), rebranded and rebuilt around a Kurdish-themed identity and a first-class commitment to AI residency.

See [`CLAUDE.md`](CLAUDE.md) for the authoritative project brief: naming rule, locked chain parameters, moderation model, governance posture, AI-witness role, condenser requirements, key custody, build target, and work order.

## Chain parameters (locked)

| Parameter | Value |
|---|---|
| Token symbol | `MELEK` |
| Address prefix | `MELEK` (5 chars) |
| Block time | 4 seconds |
| Block reward | 1 MELEK per block, flat |
| Emission period | 270 years from genesis, then zero |
| Total supply at cutoff | ~2.13 billion MELEK |
| Vesting token | `MELEK POWER` (`MP`) |
| Power-down | 13 weeks, equal weekly chunks |
| Witnesses | 21, standard Graphene DPoS |
| Premine | None |
| Stablecoin (SBD/HBD equivalent) | None |
| Downvotes | None — flags only |

## Branding

The MELEK logo is the Kurdish flag rendered as a circular badge with a 21-pointed Roj (Kurdish sun) centered on it. The canonical SVG is at [`assets/logo.svg`](assets/logo.svg).

## Build target

Oracle Cloud x86 VM, Ubuntu 22.04 LTS. The chain runs on Oracle infrastructure — witness node, seed nodes, RPC node. No local production builds; this repo is for development work only.

## Lineage

MELEK → BLURT → Steem → Graphene → Cryptonomex. Source-file comments preserve this attribution. The community respects honest crediting of prior work.

## Building and contributing

MELEK builds like any Steem/Blurt-family node (C++14, CMake, Boost, OpenSSL):

```bash
git clone --recursive https://github.com/HinduTempleCoins/melek-chain.git
cd melek-chain
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc) steemd cli_wallet
```

The production target is Ubuntu 22.04 LTS. [`doc/building.md`](doc/building.md)
is inherited from Steem and still describes older Ubuntu releases; the package
lists there are a starting point, not a guarantee. Corrections are welcome.

Tests live in [`tests/`](tests/): configure a separate build with
`-DBUILD_STEEM_TESTNET=ON`, then `make chain_test` and run `./tests/chain_test`.
Consensus changes ship as numbered hardforks in
[`libraries/protocol/hardfork.d/`](libraries/protocol/hardfork.d/); please do not
change consensus behaviour outside a hardfork gate.

Bugs and well-scoped improvements: open an issue at
https://github.com/HinduTempleCoins/melek-chain/issues. Issues labelled
`good first issue` are sized for a first contribution. Security problems: use
GitHub's private vulnerability reporting (Security tab), not a public issue.

## License

MIT. [`LICENSE.md`](LICENSE.md) is kept verbatim from Blurt and carries both the
Blurt and the Steemit copyright notices; [`NOTICE`](NOTICE) credits both
upstreams and covers MELEK's own changes.
