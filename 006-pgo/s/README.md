Exhaustive simulator
--------------------

This code exhaustively simulates a match of two teams of up to three Pokémon
per team, implementing the rules for Trainer Battles as described in "Pokémon
GO: A Quantitative Approach", including shields and substitutions.

The primary result of a run is an answer to the question "does Team 1
dominate Team 2?" Domination implies a path through the game tree such
that, no matter what the dominated team does, the dominating team wins.

The results include number of paths concluding in a victory for Team 1,
victories for Team 2, ties, and timeouts.

Interpreting the output
-----------------------

If Team 1 wins on four billion paths, and Team 2 wins on eight billion paths,
and there are no timeouts nor ties, does that mean Team 1 is a better team?
Not necessarily. For instance, paths where no charged moves are used will be
much longer than those which do use charged moves. Their forests will thus be
wider, with more total paths. It is unlikely that a match will never see
charged moves used, so these aren't particularly relevant matches, despite
probably dominating the total counts.

Nor is it sufficient to look at depth of the paths. The shortest paths will
be those on which charged moves are used optimally and shields are never
deployed. Such matches are similarly unlikely.

We produce only one clear, simple signal: whether Team 1 dominates Team 2.
