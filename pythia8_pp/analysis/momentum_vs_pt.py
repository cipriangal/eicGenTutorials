import matplotlib.pyplot as plt
import numpy as np

# 1. Define the transverse momentum (pT) range and pseudorapidity (eta) values
pt = np.linspace(0, 10, 500)  # 0 to 10 GeV/c
etas = [2.5, 3.0, 3.5, 4.0]

# 2. Set up the figure
plt.figure(figsize=(8, 5))

# 3. Calculate p = pT * cosh(eta) and plot each curve
for eta in etas:
    p = pt * np.cosh(eta)
    plt.plot(
        pt, p, linewidth=2, label=rf"$\eta = {eta}$ ($p = {np.cosh(eta):.2f} p_T$)"
    )

# 4. Customize labels, grid, and limits
plt.title(
    r"Total Momentum ($p$) vs. Transverse Momentum ($p_T$) across Forward $\eta$",
    fontsize=12,
)
plt.xlabel(r"Transverse Momentum $p_T$ (GeV/c)", fontsize=11)
plt.ylabel(r"Total Momentum $p$ (GeV/c)", fontsize=11)
plt.xlim(0, 10)
plt.ylim(0, 280)
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend(fontsize=10)

# 5. Save the figure as a PNG before showing it
plt.tight_layout()
plt.savefig("momentum_vs_pt.png", dpi=300, bbox_inches="tight")
plt.show()

