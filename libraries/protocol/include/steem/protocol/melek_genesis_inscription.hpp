#pragma once
// THE MELEK GENESIS INSCRIPTION — VERIFIED (Van Kush Family Research Institute).
// 255 bytes, pure ASCII (every char <= 127). Permanent and immutable. Locked before hashing mainnet
// genesis. Three clauses: economic (Satoshi's Bitcoin genesis headline, reused as lineage), legal
// (Serafine v. Crump — open access to the courts), temporal (the 7/12 system — seven planets/days/
// heavens, twelve signs/months/hours/tribes; Kalendae, calare, to exclaim). Block Zero, 7/12/2026.
//
// This constant is folded by SHA-256 into the mainnet chain id (see config.hpp): a node carrying a
// different inscription computes a different chain id and is, by construction, a different chain.
// Testnets do NOT use this — they use the "testnet" chain id.
#define MELEK_GENESIS_HEADLINE "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks | Serafine v. Crump, 691 S.W.3d 917 (Tex. 2024) | 7/12: Seven Planets, Seven Days, Seven Heavens; Twelve Signs, Twelve Months, Twelve Hours, Twelve Tribes - Kalendae, calare, to exclaim"
