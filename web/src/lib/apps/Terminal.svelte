<script lang="ts">
    import { onMount } from "svelte"
    import { commands } from "../os/commands"

    let input = $state('')
    let inputEl = $state<HTMLInputElement>()
    let log = $state<{ cmd: string; out: string }[]>([])

    function run() {
        const [name, ...args] = input.trim().split(/\s+/);
        const cmd = commands[name];
        const out = cmd ? cmd.run(args) : `Command not found: ${name}`;
        log.push({ cmd: input, out });
        input = '';
    }

    onMount(() => {
        inputEl?.focus()
    })
</script>

<div class="size-full overflow-y-auto bg-black/40 p-4 font-mono text-sm text-bronze-900">
    {#each log as l}
        <div> $ {l.cmd}</div>
        <pre class="mb-2 whitespace-pre-wrap">{l.out}</pre>
    {/each}

    <form onsubmit={(e) => {e.preventDefault(); run()}} class="flex gap-2">
        <span class="text-bronze-500">$</span>
        <input
            bind:this={inputEl}
            bind:value={input}
            class="flex-1 bg-transparent outline-none"
            placeholder="Enter command..."
        />
    </form>
</div>