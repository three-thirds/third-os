<script lang="ts">
    import Mist from '../lib/Mist.svelte';
</script>

<main class="relative grid min-h-screen place-items-center overflow-hidden">

    <div class="pointer-events-none absolute inset-0" aria-hidden="true">
    <div class="absolute left-1/4 top-1/4 size-[32rem] rounded-full bg-white/10 blur-3xl animate-drift"></div>
    <div class="absolute bottom-1/4 right-1/4 size-[28rem] rounded-full bg-sky-200/10 blur-3xl animate-drift [animation-delay:-9s]"></div>
  </div>

  <Mist text="three.os" class="relative text-7xl md:text-9xl [text-shadow:0_0_24px_rgb(255_255_255/0.35)]" />
</main>