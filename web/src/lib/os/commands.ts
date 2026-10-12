export type Command = {
    description: string;
    run: (args: string[]) => string;
}

export const commands: Record<string, Command> = {
    bleh: { description: "A test command", run: () => 'bleh' },
}