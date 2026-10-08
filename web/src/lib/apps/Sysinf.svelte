<script lang="ts">
    let reveal = $state(false)

    const things: [string, string][] = [
        ['Kernal', 'leaf 6.7.9-arch1-1'],
        ['Build', '26100.autumn.261007-1620'],
        ['Architecture', 'x86-64'],
        ['Processor', `${navigator.hardwareConcurrency ?? '?'} logical cores`],
        ['Display', `${screen.width}x${screen.height}`],
        ['Shell', 'turtle 116']

    ]
</script>

<div class="space-y-2 p-5 font-doto cursor-default">
    <p class="text-xl text-bronze-700 font-bold uppercase tracking-wide">three.os</p>
    {#each things as [name, value]}
        <div class="flex items-center justify-between text-sm font-mono">
            <p class="text-bronze-700/80">{name}</p>
            <p class="text-bronze-700/60">{value}</p>
        </div>
    {/each}
    <div class="flex items-center justify-between text-sm font-mono" >
        <p class="text-bronze-700/80">S/N</p>
        <button class="text-bronze-700/60" onclick={() => (reveal = !reveal)}>84A6B604D6C69636C2</button>
    </div>
    <button class="text-sm">version 0.1.67</button>
    {#if reveal}
    <p class="text-sm">Hey, try <b>whatevery i need</b></p>
    {/if}
</div>