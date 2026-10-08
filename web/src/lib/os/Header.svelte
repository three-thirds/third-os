<script lang="ts">
    import {SvelteDate} from 'svelte/reactivity';
    const date = new SvelteDate();
    const formatter = new Intl.DateTimeFormat(undefined, {
        hour: 'numeric',
        minute: 'numeric',
        second: 'numeric'
    })
    $effect(() => {
        const interval = setInterval(() => 
            date.setTime(Date.now()), 1000)
        return () => clearInterval(interval)
    })
</script>

<header class="relative z-30 flex h-8 items-center justify-between bg-rich-mahogany-300/75 px-4 font-mono text-sm text-bronze-900 backdrop-blur-md border-bronze-500/30">
    <div>
        <p class="font-bold uppercase tracking-wider font-doto">Third.os</p>
    </div>
    <div>
        <p>{formatter.format(date)}</p>
    </div>
</header>