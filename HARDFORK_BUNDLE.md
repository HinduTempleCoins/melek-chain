# MELEK hardfork bundle — HF25 / HF26 / HF27 + the Move attester flip

**Status: STAGED. Built, not compiled, not deployed, not activated.**
Branch: `feat/hf27-content-rewards-null-feed` (stacks linearly on `feat/hf26-remove-downvotes`).

The live chain is at **HF24**, `hardfork_version 0.24.0`, head ~1.44M. Everything below is in the
tree and gated; none of it is live. Three numbered hardforks and one time-gated flip have
accumulated while parked. They should go out **together, in one coordinated deploy**, because
every one of them needs the same thing: all five witness nodes running the same binary before any
gate opens.

---

## 1. What is in the bundle

| | What it does | Gate | Current value | Must change? |
|---|---|---|---|---|
| **HF25** | Native-EVM consensus-surface freeze. Reserves `evm_deposit`/`evm_withdraw`/`evm_call` op tags, SPACE_ID-21 state objects, the `evm_state_checkpoint` required-action. **Every body is a no-op in P0** and there is zero evmone dependency — this only reserves the surface so the eventual turn-on is a plugin change, not a state migration. | `STEEM_HARDFORK_0_25_TIME` | `1924992000` (2031-01-01) **placeholder** | **YES** |
| **HF26** | Removes downvotes. Gates the negative-vote rejection in `vote_evaluator`/`vote2_evaluator` and retires the downvote mana pool (`downvote_pool_percent → 0`). Turns the already-declared "flags, no downvotes" posture into an actual consensus rule. | `STEEM_HARDFORK_0_26_TIME` | `1924992000` (2031-01-01) **placeholder** | **YES** |
| **HF27** | **Content & curation rewards fix.** Stops the null MBD feed zeroing every payout. See §2. | `STEEM_HARDFORK_0_27_TIME` | `1924992000` (2031-01-01) **placeholder** | **YES** |
| **Move attester** | Pays walkers from the chain's `move` reward fund via an attester `custom_json`, instead of via posts or out of hathor's wallet. Not a numbered hardfork — a time-gated flip. | `MELEK_MOVE_ATTESTER_PAY_TIME` | `1785817800` (**2026-08-04 — IN THE PAST**) | **YES — URGENT** |

### ⚠️ The Move attester gate is the dangerous one

`MELEK_MOVE_ATTESTER_PAY_TIME` is **already past**. Unlike the three placeholders — which are set
so far in the future that an un-bumped deploy simply does nothing — this one **activates the
instant a node restarts on the new binary**. During a rolling restart that means upgraded and
un-upgraded nodes immediately disagree about the move payout and **fork apart**.

**It must be bumped to the same coordinated window as the hardforks before anything is built.**
The staging doc (`Bot/.local/HF25_MOVE_ATTESTER_PAYOUT.md`) already warned about exactly this;
the gate has simply gone stale since August.

---

## 2. HF27 — why content rewards have never worked

**No author or curation reward has ever been paid on MELEK mainnet.** Every account reports
`posting_rewards: 0` and `curation_rewards: 0` — hathor, kalivankush, and every user — since
genesis on 2026-07-12. Posts reach cashout normally and credit nothing. The `post` reward fund has
grown past **730,000 MELEK** because nothing ever leaves it.

The cause is in `cashout_comment_helper()`:

```cpp
if( util::to_sbd( current_steem_price, asset( reward, STEEM_SYMBOL ) ) < STEEM_MIN_PAYOUT_SBD )
   reward = 0;                                           // 0.000 MBD < 0.020 MBD -> always true
uint64_t max_steem = util::to_steem( current_steem_price, comment.max_accepted_payout ).amount.value;
reward = std::min( reward, max_steem );                  // max_steem == 0         -> always zero
```

`util::to_sbd()` and `util::to_steem()` both **return zero when the price is null**. So the payout
is zeroed twice over, independently.

**And the price can never be non-null.** `update_median_feed()` only writes a median when
`feeds.size() >= STEEM_MIN_FEEDS`, and MELEK defines `STEEM_MIN_FEEDS` as `STEEM_MAX_WITNESSES/3`
= **7** (stock Steem uses `/7` = 3). The chain has **five** witness accounts —
`num_scheduled_witnesses: 5`. At most five feeds can ever be collected. **5 < 7, permanently.**
No witness has ever published a feed either (`last_sbd_exchange_update` = 1970-01-01 on all five),
but publishing would not help: even all five publishing cannot reach the quorum.

This is vestigial Steem machinery on a chain that deliberately has no second token. From
`CLAUDE.md`: *"Single token: MELEK is the only on-chain token. No SBD/HBD/BBD equivalent, no
stablecoin pair, no internal currency conversion logic."* `current_sbd_supply` is `0.000 MBD` and
`STEEM_SBD_START_PERCENT_HF20` is `0`, so no MBD exists or can ever be printed.

The chain has therefore been violating its own stated design — *"every MELEK in circulation must
be earned through block production, content rewards, or curation rewards"* — since launch. All
1,904,744 MELEK in supply came from block production alone.

**HF27 skips both MBD-denominated guards when the price is null**, preserving only the piece that
still has meaning: `max_accepted_payout == 0` means the author declined payout.

### Not retroactive

Comments already settled at zero **stay settled**. A payout cannot be re-run after the fact. Only
comments whose cashout falls at or after the HF27 block can pay. Making past authors whole, if you
want to, is a manual transfer exercise off-chain.

**This is what the activation time buys.** As of 2026-10-05T13:40Z there were **159 posts inside
the 7-day window across 15 authors, carrying 3,249.592 MELEK of pending payout**. They settle to
permanent zero on a rolling basis — roughly one batch per day. Every day of delay is another day
of posts zeroed forever.

### Follow-up: the condenser will still show $0

`comment_object.total_payout_value` and `curator_payout_value` are hard-typed `asset(0, SBD_SYMBOL)`
and serialized, so they are written through `to_sbd()` and will **keep reading `0.000 MBD`** even
after HF27 pays real rewards. Changing their symbol is a state migration and is deliberately NOT
part of this bundle.

The real, correct value is already on-chain: **`comment_object.author_rewards`** (a `share_type`
in MELEK, incremented with `author_tokens`), plus the account-level `posting_rewards` /
`curation_rewards` counters. **The condenser must be changed to display those** — otherwise
rewards will land and users will still see zero. That is a front-end task, tracked separately.

---

## 3. Ordering and the sanity check

`process_hardforks` applies hardfork numbers **sequentially** — HF27 cannot apply before HF26,
which cannot apply before HF25. Set the three times **equal, or strictly ascending**. If equal,
all three apply in order within the same block, which is fine and is the simplest option.

`init_hardforks` asserts `STEEM_BLOCKCHAIN_HARDFORK_VERSION == versions[STEEM_NUM_HARDFORKS]`, so
`STEEM_NUM_HARDFORKS` (now **27**) and `STEEM_BLOCKCHAIN_VERSION` (now **0.27.0**) must stay in
step with the top `.hf` file. Both are updated on this branch.

---

## 4. Deploy sequence (nothing below has been done)

1. **Set the four gate times.** Three `STEEM_HARDFORK_0_2{5,6,7}_TIME` and
   `MELEK_MOVE_ATTESTER_PAY_TIME`. Pick a window safely **after** the rolling restart completes —
   a few hours of margin, not minutes.
2. **Build on the chain host.** Not in this repo and not in a codespace; the binary lives on its
   own VPS (`Bot/CLAUDE.md`: *"Don't run the `witness_node` binary from this repo"*).
3. **Testnet first**, where every gate is `1` (`IS_TEST_NET`) and therefore active immediately on
   the upgraded binary. Confirm a post actually pays out.
4. **From-genesis replay** on mainnet data with the new binary, before any witness runs it. This is
   the check that the gating is right: a replay must reproduce history exactly, including the zero
   payouts that really happened. If the replay diverges, the gating is wrong — stop.
5. **Roll the five witness nodes** — `melek-mainnet` runs `initminer` + `hathor`;
   `melek-witnesses` runs `maat` + `seshat` + `thoth` — plus the read-only RPC node. All on the new
   binary **before** the gate time.
6. **Watch the gate block.** Then confirm a cashout credits `posting_rewards`.

### Fold in while the nodes are down

**Account history is dead and needs a replay anyway.** `condenser_api.get_account_history` returns
`[]` for every account including hathor; the rocksdb store holds one 741-byte `.sst` from Sep 15
and a 0-byte WAL. That is why the wallet shows no reward history — and it would still show none
after HF27. The replay in step 4 rebuilds it. `melek-witnesses` already runs a separate read-only
RPC node that can serve the front end while the producing nodes are busy.

---

## 5. What is NOT in this bundle

- **Removing MBD/SBD entirely.** Assessed and deliberately deferred — the asset is already inert
  (supply 0, feed dead, interest 0) and dropping a serialized asset is a much heavier state
  migration. HF27 routes around it instead.
- **Lowering `STEEM_MIN_FEEDS`, or adding witnesses to reach 7.** Either would let a median feed
  exist, but both mean inventing a price in a currency that has no supply and can never be printed.
  Routing around the dead feed is the honest fix.
- **Any condenser change.** See the follow-up note in §2.
