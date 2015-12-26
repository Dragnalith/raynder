= Raynder =

== TO DO ==
- [x] Use a better random generator
- [x] Use gamma 2.2 for float to integer conversion
- [x] Fix firefly
- [x] Fix anti-firefly (some pixel remain dark). It comes from NAN value.
- [x] Implement Russian Roulette termination
- [x] Importance sample at least according to "cos(theta)".
- [x] Fix black light bug (because the first ray from camera does not sample the light)
- [x] Next event estimation
- [ ] Profile and optimize code
- [ ] Add tone mapping
- [ ] Add other BSDF (reflection, refraction)
- [ ] Remove useless SFML dependencies (audio, network, ...)

Note: Next event estimation has bug. There are firefly. After a quick investigation
radiance leaving the ceiling very close to the light is higher than the radiance
of the light. It comes from the 1 / (d*d) factor. When distance to the light is very
low, radiance become very high. But it cannot be higher than the radiance of the light
itself. So math are wrong somewhere.
