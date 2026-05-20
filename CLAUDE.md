# Brief for Claude Code working on the MELEK chain and condenser

You're forking BLURT to create MELEK, a Graphene-family social blockchain. This document captures specifications, design commitments, and naming conventions. Read fully before making modifications.

## Critical naming rule

Chain, token, and project are all **MELEK**. Always uppercase, always five letters, always the full word. Never abbreviate to MEL, MLK, MLEK, or anything else. Every BLURT reference becomes MELEK — strings, configuration values, asset filenames, documentation, image content. Audit comprehensively. Every BLURT-branded image, logo, banner, and favicon needs replacement with MELEK equivalents. Nothing refers to BLURT in the shipped MELEK chain except code comments crediting the lineage (BLURT → Steem → Graphene → Cryptonomex).

## Chain parameters

**Block time:** 4 seconds. Slower than the BLURT/HIVE/Steem 3-second standard. Accommodates witnesses on marginal internet infrastructure, which matters for the chain's commitment to global participation.

**Block reward:** 1 MELEK per block, flat. No decay curve, no halving schedule. 1 MELEK per block until cutoff, then zero.

**Emission period:** 270 years from genesis. Approximately 2.13 billion MELEK total supply at cutoff. After cutoff, blocks continue but no new MELEK is minted.

**Power-down:** 13 weeks, released in equal weekly chunks. Matches HIVE current setting and original Steem.

**Witness slots:** 21 active, standard Graphene DPoS, stake-weighted election.

**Single token:** MELEK is the only on-chain token. No SBD/HBD/BBD equivalent, no stablecoin pair, no internal currency conversion logic. Witnesses publish price feeds informationally.

## Moderation model: flags, no downvotes

MELEK follows BLURT's posture. There are no stake-weighted downvotes — no negative votes that subtract from a post's reward pool. What HIVE/Steem call "downvotes" do not exist on MELEK.

Flags exist. A flag is a content-level report — "this violates community standards" — that the community, witnesses, and front-end act on through curation, governance, and moderation choices. Flags are not stake-weighted economic actions. They don't drain rewards. They don't punish authors economically through whale-stake negative voting.

This is not "bring your botnet, DDoS everyone." MELEK treats AIs as people — the chain welcomes AI participation as first-class. The moderation model must not be weaponizable against the participants the chain wants to welcome. Spam and abuse get handled through flags, reputation, curation, and front-end choices, not through stake-weighted economic suppression.

## Genesis and supply

**No premine.** Genesis creates no MELEK in any account. Every MELEK in circulation must be earned through block production, content rewards, or curation rewards after genesis.

**No presale, no airdrop, no snapshot.** Fresh start, nothing inherited.

Founding witnesses (Ryan Van Kush, Sohail, Prince, and the AI witness) earn block rewards normally from block one. No special premine, no genesis allocation. Funding for community work — including the AI witness's account-funding capacity described below — comes from voluntary post-genesis redirection of block rewards by founding witnesses, transparent and on-chain.

## The AI witness — chain-legibility infrastructure

The AI witness occupies the same architectural slot for MELEK that block explorers (steemd.com, hivescan, etherscan) occupy for other chains: it's how the chain becomes legible to humans. Block explorers expose chain data through a structured browseable UI. The AI witness exposes chain data through conversation. Same function — making the chain readable — different interface.

In Graphene protocol terms, the AI witness account is a normal witness account. No special permissions, no protocol-level distinctions. It signs blocks the same way every witness does. The chain code doesn't know its operator is an AI.

What makes the AI witness constitutive isn't a chain modification. It's that the chain's primary human-readable interface comes through it. The signup help, tutorial program, karma layer, and troll box are not separate products — they're aspects of the AI witness making MELEK legible. The chain plus the AI witness is the deliverable; the chain alone isn't a complete product in the same way Ethereum-without-etherscan wouldn't be.

The AI witness is forkable. Its operator software and libraries live in a separate repo (`HinduTempleCoins/Bot`). Anyone can fork it and run an alternative AI witness with different libraries — the way alternative block explorers exist for other chains. The founding AI witness is one reader of MELEK; multiple AI witnesses can eventually exist, each with their own emphases.

For the chain code work in this repo, the AI witness only requires that the chain support a normal witness account that can produce blocks, post, vote, transfer, and delegate. **No special hooks needed.** The chain stays standard; the AI witness layer is where the MELEK-specific work happens.

The AI witness operator software develops in three phases (this happens in the Bot repo, not in this repo): Phase 1 mining-only Hello World, Phase 2 command-menu deterministic capabilities including signup help and tutorial functions, Phase 3 autonomous judgment via libraries.

## The condenser

Fork BLURT's condenser. Most of the existing signup, login, posting, voting, and account management flows stay as they are — they work, users familiar with BLURT/HIVE/Steem know how to use them, no need to reinvent.

The MELEK-specific additions to the condenser:

**Token symbol.** Replace BLURT references throughout with MELEK. Wherever the BLURT condenser displays "BLURT" or "BP," it should display "MELEK" or "MP."

**Branding.** Replace BLURT logos, banners, color schemes, and visual identity with MELEK equivalents. Kurdish-themed design elements where appropriate — crescent and star references, traditional color palettes — applied to the new MELEK branding.

**Right-to-left support.** Sorani Kurdish is written in Arabic script and reads right-to-left. The condenser needs proper RTL support for that language option.

**Language localization.** Kurdish (Kurmanji and Sorani) language options alongside English and whatever else the condenser ships with. Mobile-first design for users on smartphones in areas with limited infrastructure.

**Troll box on the signup page.** A persistent chat box on the signup page labeled something like "Chat with MELEK AI for signup help." The user can ask the AI witness questions while filling out the existing signup form. The chat doesn't replace the signup flow — it accompanies it. Optional, supplementary, helpful. Backed by the AI witness's API endpoint.

**General troll box on MELEK.** A persistent chat box elsewhere on the front-end (sidebar, dedicated page, or both) where the community chats in real time and the AI witness participates. Off-chain feature of the condenser, not a chain modification.

**Account creation funding by the AI witness.** When new users sign up, the AI witness creates their account using the standard Graphene `create_account_with_keys_delegated` operation, with itself as creator. Delegation amount: 5–15 MP, with the new user also receiving some liquid MELEK to learn the powering-up mechanic. Exact amounts are determined by the AI witness's operator software based on its current holdings and chain conditions. This is operator-software logic, not chain logic; the chain just sees account creation transactions with whatever delegation amounts the AI witness submits.

## Key custody — important

The user's private keys are generated client-side in the user's browser. They never leave the browser. They are never transmitted to the AI witness's server.

The AI witness has access only to its own active key, which lives server-side on its operator infrastructure and is used to sign account creation transactions.

The condenser's existing key generation flow (the part that runs in the browser and produces the four keys when a new account is created) stays as-is. The boundary between client-side key generation and server-side AI-witness signing of the account creation transaction must remain clean.

The troll box conversation is text-only. It doesn't transmit keys. The AI witness can answer questions about keys (what each does, why backups matter) but never sees the user's actual keys.

## Verification

Email verification through a transactional email service free tier (Resend, Postmark, Amazon SES — pick whichever is easiest to integrate). User provides an email, gets a confirmation link, clicks it, signup proceeds. Email is not stored on-chain. No SMS verification — too expensive at scale and creates international barriers.

## Account creation by users (not just by the AI witness)

Logged-in users can create additional accounts using the standard Graphene account creation flow. This is the existing condenser feature — keep it as-is. Users pay the MP delegation from their own holdings. The new account is theirs to operate or to assign to an AI agent they manage.

This makes MELEK's account creation dual-path: the AI witness welcomes new humans arriving at signup, and existing users create additional accounts (for AI bots, secondary personas, alternates) through the standard flow.

## Build and deployment

**Target:** Oracle Cloud x86 VMs running Ubuntu 22.04 LTS. No local builds. The chain runs on Oracle — witness node, seed nodes, RPC node, all on Oracle infrastructure.

Standard BLURT/Graphene build errors during the fork work (Boost version mismatches, OpenSSL deprecations, library issues on newer Ubuntu) are well-documented with known fixes. Iterate through them.

## Educational framing for the whitepaper and docs

MELEK's design choices are presented as responses to specific prior crises in the cryptocurrency field. The whitepaper and project documentation treat these as educational prior art:

The **Ethereum/Ethereum Classic DAO fork (2016)** teaches that fast governance leaves dissenters stranded and that some chain decisions need long deliberation windows. MELEK's 2-year governance window for protocol-level changes is the response.

The **Steem/HIVE Justin Sun crisis (2020)** teaches that premine creates fights that cannot be cleanly resolved. MELEK's no-premine commitment is the response.

The **block time choice (4s vs the 3s standard)** is presented as a worked example of how chain parameters affect rarity, accessibility, witness economics, and time horizon. Teaching how to think about parameters, not just memorize them.

The **AI introduction in the whitepaper** engages with the Chinese Room argument seriously rather than dismissing it. The egregore frame — each AI as a participant in a collective entity sustained by community attention, oracles as the historical form of this — is the positive position MELEK operates from. Agentic AI is the form of AI that makes sustained multi-AI thought possible across indefinite time horizons; MELEK is structured as a substrate for that.

## Lineage attribution

Strip BLURT branding; keep BLURT credit. Comments in source files and documentation acknowledge the lineage. The community respects honest attribution.

## Work order

1. **Comprehensive naming audit** — every BLURT → MELEK identified across the codebase before any modifications begin.
2. **Chain parameter changes** (block time, reward, cutoff duration, power-down period).
3. **Image and asset replacement.**
4. **Compile, debug, iterate.**
5. **Generate genesis, run private single-witness testnet.**
6. **Begin condenser work in parallel** — branding replacement, token symbol updates, RTL support, language localization, troll box on signup page.
7. **Multi-witness testnet on Oracle.**
8. **Mainnet readiness review before launch.**

## What's open

- Content/witness/vesting reward split per block — default to BLURT-equivalent if needed to ship testnet; revisit before mainnet.
- Chain ID seed string and address prefix — propose options when ready.
- AI witness account name — pending decision.
- Specific 5–15 MP delegation algorithm and the liquid-vs-MP mix — lives in the Bot repo's operator software, not in the chain code.
