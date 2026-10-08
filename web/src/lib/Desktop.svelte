<script lang="ts">
    import Leavesinf from './Leaves_infinite.svelte';
    import Dock from '../lib/os/Dock.svelte';
    import Window from '../lib/os/Window.svelte';
    import { wm } from '../lib/os/windows.svelte';
</script>


<div class="fixed inset-0 flex flex-col">
    <div
        class = "absolute inset-0 bg-cover bg-center"
        style="background-image: url('https://cdn.hackclub.com/01a11608-a876-780b-86ff-44d23e9d701b/360_F_617793807_ogJx15srsWoYxObjTsjDfZ3xrOJrjvqG.jpg')"
        aria-hidden="true"
    ></div>

    <div class="absolute inset-0 bg-rich-mahogany-100/50" aria-hidden="true"></div>
    <Leavesinf count={67} />

    <header class="relative z-30 flex h-8 items-center justify-between bg-rich-mahogany-100/70 px-4 font-doto text-sm text-bronze-700 backdrop-blur">
        hi boo idk what to put her but heder prob like clock and stuff idk copy my dotfiles
    </header>

    <div class="relative z-10 flex-1"></div>

    <Dock />

    <div class="pointer-events-none fixed inset-0 z-20">
        {#each wm.wins as win (win.id)}
            <Window {win} />
        {/each}
    </div>
</div>

