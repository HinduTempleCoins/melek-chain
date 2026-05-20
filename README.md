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

## License

See [`LICENSE.md`](LICENSE.md).
