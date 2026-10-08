<script lang="ts">
    let reveal = $state(false)

    const things: [string, string][] = [
        ['Kernal', 'leaf 6.7.9-arch1-1'],
        ['Build', '26100.autumn.261007-1620'],
        ['Architecture', 'None'],
        ['Processor', `${navigator.hardwareConcurrency ?? '?'} logical cores`],
        ['Display', `${screen.width}x${screen.height}`],
        ['Shell', 'turtle 116']

    ]
</script>

<div class="space-y-2 p-5 font-doto cursor-default">
    <p class="text-xl text-bronze-700 font-bold uppercase tracking-wide">three.os</p>
    {#each things as [name, value]}
        <div class="flex items-center justify-between text-sm">
            <p class="text-bronze-700/80">{name}</p>
            <p class="text-bronze-700/60">{value}</p>
        </div>
    {/each}
    <button class="text-sm" onclick={() => (reveal = !reveal)}>version 0.1.14</button>
    {#if reveal}
    <p class="text-sm">Hey, try <b>Cmd + R</b></p>
    {/if}
</div>