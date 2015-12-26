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
- [ ] Remove useless SFML dependencies (audio, network, ...)
- [ ] Add other BSDF (reflection, refraction)
- [ ] Improve variance:
    - try stratified sampling (http://www.rickdebruijne.nl/data/pathtracer/literature_research.pdf)
    - Review current next event estimation implementation from Multiple Importance Sampling point of view
- [ ] Add tone mapping
- [ ] Load object from files
- [ ] Bidirectional Path-tracing
- [ ] Profile and optimize code
