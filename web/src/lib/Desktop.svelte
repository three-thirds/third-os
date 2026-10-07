<script lang="ts">
    import Leavesinf from './Leaves_infinite.svelte';
</script>


<div class="fixed inset-0 flex flex-col">
    <div
        class = "absolute inset-0 bg-cover bg-center"
        style="background-image: url('https://cdn.hackclub.com/01a11608-a876-780b-86ff-44d23e9d701b/360_F_617793807_ogJx15srsWoYxObjTsjDfZ3xrOJrjvqG.jpg')"
        aria-hidden="true"
    ></div>

    <div class="absolute inset-0 bg-rich-mahogany-100/50" aria-hidden="true"></div>
    <Leavesinf count={67} />
</div>

