# Hack Machine VM Translator
Tradutor de código interino produzido pelo Jack Compiler para [Hack Machine Assembly](https://github.com/RenanGalvao/hack-machine-assembler). 
Feito para o projeto #7 do curso [Nand2Tetris](https://www.nand2tetris.org/) em C.

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

## Tabela de Conteúdo
- [Hack Machine VM](#hack-machine-vm)
    - [Mapeamento da RAM](#mapeamento-da-ram)
    - [Comandos](#comandos)
        - [Push/Pop](#push-%2F-pop)
        - [Lógico-Aritiméticos](#lógico-aritiméticos)
- [Uso](#uso)
- [Cobertura de testes](#cobertura-de-testes)
- [Autores](#autores)
- [Licença](#licença)
- [Notas](#notas)

## Hack Machine VM
Uma [stack machine](https://en.wikipedia.org/wiki/Stack_machine), feita para o Hack Computer, com 
apenas um tipo de dado: inteiro de 16 bits com sinal[^1]. Um programa nessa VM, é uma sequência de 
comandos separados por linhas num arquivo ``Program.vm``, cada comando pode estar numa categoria:

- push/pop
- lógico-aritiméticos
- controle de fluxo
- funcões

### Mapeamento da RAM
Os segmentos ``local``, ``argument``, ``this`` e ``that`` são mapeados dinâmicamente no Hack Computer.
O segmento ``pointer`` possui duas possibilidades: 0 e 1, que são mapeadas respectivamente para ``this``
 e ``that`` ou ``RAM[3]`` e ``RAM[4]``, que são a implementação concreta no Hack Computer. O segmento 
``temp`` também é fixo e mapeado nos endereços ``RAM[5]`` a ``RAM[12]``. O segmento ``constant`` é um
segmento virtual e portanto não é mapeado para nenhum espaço físico na memória. O segmento ``static``
é mapeado nos endereços ``RAM[16]`` a ``RAM[255]``.

<details>
    <summary>ver imagens</summary>

![ram-usage-0](assets/ram-usage-0.png)
![ram-usage-1](assets/ram-usage-1.png)

*base address*
: é o endereço concreto no Hack Computer.
</details>

### Comandos

#### Push / Pop
push *segment index*
: move o valor de ``segment[index]`` para a *stack*, onde *segment* pode ser ``argument``, ``local``, 
``static``, ``constant``, ``this``, ``that``, ``pointer`` ou ``temp`` e *index* é um número não negativo. 

<br/>

pop *segment index*
: faz o caminho inverso de *push*: retira da *stack* e move para ``segment[index]``, onde *segment* pode
ser os mesmos que no caso de push, com exceção de ``constant`` que é um segmento virtual.

#### Lógico-Aritiméticos
- aritiméticos: ``add``, ``sub``, ``neg``
- comparação: ``eq``, ``gt``, ``lt``
- lógicos: ``and``, ``or``, ``not``

<details>
    <summary>ver imagem</summary>

![arithmetic-logical-commands](assets/arithmetic-logical-commands.png)
</details>


## Uso
- Tenha o [Ceedling](https://www.throwtheswitch.org/ceedling#get-ceedling-section) instalado.
- Obtenha uma cópia do repositório ``git clone https://github.com/RenanGalvao/hack-machine-vm-translator``.
- Entre na pasta e execute o build ``cd hack-machine-vm-translator && ceedling release``.

Para gerar o código ``asm``, utilize dentro da pasta raiz: ``./build/release/vm-translator <source-file>``. 
Caso não tenha nenhum tipo de erro no código fonte, o arquivo ``.asm`` estará na mesma pasta que o código fonte.

## Cobertura de Testes
<table class="legend">
    <tbody><tr>
      <th scope="row">Directory:</th>
      <td>src/</td>
    </tr>
    <tr>
      <th scope="row">Date:</th>
      <td>2025-03-26 20:33:00</td>
    </tr>
    <tr>
      <th scope="row">Coverage:</th>
      <td class="legend">
        <span class="coverage-low">low: ≥ 0%</span>
        <span class="coverage-medium">medium: ≥ 75.0%</span>
        <span class="coverage-high">high: ≥ 90.0%</span>
      </td>
    </tr>
  </tbody></table>

<table class="coverage">
    <tbody><tr>
      <th></th>
      <th scope="col">Exec</th>
      <th scope="col">Total</th>
      <th scope="col">Coverage</th>
    </tr>
    <tr>
      <th scope="row">Lines:</th>
      <td>319</td>
      <td>337</td>
      <td class="coverage-high">94.7%</td>
    </tr>
    <tr>
      <th scope="row">Functions:</th>
      <td>25</td>
      <td>27</td>
      <td class="coverage-high">92.6%</td>
    </tr>
    <tr>
      <th scope="row">Branches:</th>
      <td>114</td>
      <td>126</td>
      <td class="coverage-high">90.5%</td>
    </tr>
  </tbody></table>

<table class="file-list">
  <colgroup><col>
  </colgroup><colgroup span="3">
  </colgroup><colgroup span="2">
  </colgroup><colgroup span="2">

  </colgroup><tbody><tr>
    <th scope="col">File</th>
    <th scope="colgroup" colspan="3">Lines</th>
    <th scope="colgroup" colspan="2">Functions</th>
    <th scope="colgroup" colspan="2">Branches</th>
  </tr>


  <tr>
    <th scope="row">
      code.c
    </th>
    <td>
      <meter class="coverage-high" min="0" max="100" value="95.5" title="95.5%">95.5</meter>
    </td>
    <td class="CoverValue line-coverage coverage-high">95.5%</td>
    <td class="CoverValue line-coverage coverage-high">168 / 176</td>
    <td class="CoverValue function-coverage coverage-medium">83.3%</td>
    <td class="CoverValue function-coverage coverage-medium">10 / 12</td>
    <td class="CoverValue branch-coverage coverage-high">95.2%</td>
    <td class="CoverValue branch-coverage coverage-high">40 / 42</td>
  </tr>


  <tr>
    <th scope="row">
      parser.c
    </th>
    <td>
      <meter class="coverage-high" min="0" max="100" value="96.7" title="96.7%">96.7</meter>
    </td>
    <td class="CoverValue line-coverage coverage-high">96.7%</td>
    <td class="CoverValue line-coverage coverage-high">59 / 61</td>
    <td class="CoverValue function-coverage coverage-high">100.0%</td>
    <td class="CoverValue function-coverage coverage-high">2 / 2</td>
    <td class="CoverValue branch-coverage coverage-high">97.4%</td>
    <td class="CoverValue branch-coverage coverage-high">37 / 38</td>
  </tr>


  <tr>
    <th scope="row">
      utils.c
    </th>
    <td>
      <meter class="coverage-high" min="0" max="100" value="92.0" title="92.0%">92.0</meter>
    </td>
    <td class="CoverValue line-coverage coverage-high">92.0%</td>
    <td class="CoverValue line-coverage coverage-high">92 / 100</td>
    <td class="CoverValue function-coverage coverage-high">100.0%</td>
    <td class="CoverValue function-coverage coverage-high">13 / 13</td>
    <td class="CoverValue branch-coverage coverage-medium">80.4%</td>
    <td class="CoverValue branch-coverage coverage-medium">37 / 46</td>
  </tr>

</tbody></table>
Generated by: GCOVR (Version 5.2)

## Autores
- [@RenanGalvao](https://www.github.com/RenanGalvao)


## Licença
[GPL-3.0](LICENSE)


[^1]: Os valores de *true* e *false* são representados respectivamente por -1 e 0.
